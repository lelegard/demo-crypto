//
// Encrypt an image file using ECB and CBC mode.
// Reproduce chaining mode illustration from
// http://en.wikipedia.org/wiki/Block_cipher_mode_of_operation
//
// External library requirements (including development files):
// - Magick++ (ImageMagick library for C++)
// - OpenSSL
//
// Thierry Lelegard, 2015-01-28
//

#include <iostream>
#include <vector>
#include <Magick++.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void encryptImage(Magick::Image& inImage, const EVP_CIPHER* evp, const std::string imageFormat, const std::string& outFileName)
{
    // Get cipher characteristics.
    const std::string cipherName(EVP_CIPHER_name(evp));
    const size_t cipherBlockSize(EVP_CIPHER_block_size(evp));
    const size_t cipherKeySize(EVP_CIPHER_key_length(evp));
    const size_t cipherIvSize(EVP_CIPHER_iv_length(evp));

    // Get image characteristics.
    const Magick::Geometry size(inImage.size());
    const size_t width(size.width());
    const size_t height(size.height());

    std::cout << "Creating " << outFileName
              << " using " << cipherName
              << ", key size: " << cipherKeySize
              << ", block size: " << cipherBlockSize
              << ", IV size: " << cipherIvSize
              << ", image size: " << width << "x" << height
              << " (" << imageFormat << ")"
              << std::endl;

    // Encryption key and IV. Do not use all identical bytes in key because of TDES!
    std::vector<unsigned char> key(cipherKeySize);
    std::vector<unsigned char> iv(cipherIvSize, 0x00);
    for (size_t i = 0; i < key.size(); ++i) {
        key[i] = i;
    }

    // Write image as an array of raw pixels data.
    std::vector<unsigned char> inPixels(width * height * imageFormat.length());
    inImage.write(0, 0, width, height, imageFormat, Magick::CharPixel, &inPixels[0]);
        
    // Create output pixels area, provision space for optional padding.
    std::vector<unsigned char> outPixels(inPixels.size() + 2 * cipherBlockSize);

    // Encrypt raw pixels data.
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int cipherSize = 0;
    if (EVP_EncryptInit(ctx, evp, &key[0], &iv[0]) != 1 ||
        EVP_EncryptUpdate(ctx, &outPixels[0], &cipherSize, &inPixels[0], inPixels.size()) != 1 ||
        EVP_EncryptFinal(ctx, &outPixels[cipherSize], &cipherSize) != 1)
    {
        // EVP error
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    EVP_CIPHER_CTX_free(ctx);
    
    // Create an image from encrypted raw pixels.
    Magick::Image outImage(width, height, imageFormat, Magick::CharPixel, &outPixels[0]);
    outImage.write(outFileName);
}


int main (int argc, char* argv[])
{
    // Need one argument: input file.
    if (argc != 2) {
        std::cerr << "Syntax: " << argv[0] << " input-file" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string inputFile(argv[1]);

    // Initialize ImageMagick.
    Magick::InitializeMagick(*argv);

    // Initialize OpenSSL
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    try {
        // Load input image.
        Magick::Image image(inputFile);

        // Encrypt image in various modes.
        encryptImage(image, EVP_aes_128_ecb(), "RGB",  inputFile + ".rgb.aes.ecb.jpg");
        encryptImage(image, EVP_aes_128_cbc(), "RGB",  inputFile + ".rgb.aes.cbc.jpg");
        encryptImage(image, EVP_aes_128_ecb(), "RGBA", inputFile + ".rgba.aes.ecb.jpg");
        encryptImage(image, EVP_aes_128_cbc(), "RGBA", inputFile + ".rgba.aes.cbc.jpg");
    } 
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl; 
        return EXIT_FAILURE;
    } 

    EVP_cleanup();
    ERR_free_strings();
    return EXIT_SUCCESS;
}
