#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "cryptopp/cryptlib.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha3.h"
#include "cryptopp/md5.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/pssr.h"
#include "cryptopp/secblock.h"
#include "cryptopp/files.h"

#include "cpuid.h"

using namespace std;
using namespace CryptoPP;

#define HASH_ALGO SHA3_256
#define ZERO_BYTES 3
#define LI_SIZE 16
#define SALT_1 "12345678901234567890123456789012"
#define SALT_2 "12345678901234567890123456789012"

#define PUBLIC_KEY "308204BB020100300D06092A864886F70D0101010500048204A5308204A10201000282010100B7A5F93E915C1C6E4BBC40467697F90FF3BD6917F8FA247D3338A2565FDB6A0DC957A2C20DBC13ADB37C30A901756B2387EAE3D3D6ED294AFF46FD5FA679AB8857F3326D35FB94913A9098077A4B64EDDE933DA62B72D03D277D94E3DD7B2D987F2E5FEA0DD844B4C4DEFFAF5A606DD5A85E8951BB44DF4468B039770B70C83D8DAECAA72076DAF8BFCF4F4F8508F0D67F84809313822CD7E1D4184F8822DCB5A537C5813EFDE88C3A8805F1C7B6C3561F3548DCA2B2DB41CBCAC3E720D68823BDDD4FE2864FA0D7020BA2C18320864A9B8B4C996066628ED2C46B9915206846C733175BD710541A5614408AADB109934CC1AE1CF55301319CF679952777CC33020111028201000566C390E6285B306BA3A7899A1387534AEEFB8FC38EE2F49F9FC88A11E0CE69D137488D3CA3A639D092B62316A19235C03B9D49FECABD7325A034A0EE4EDF65E4781812455A3913548BC83C7489BF342C3181D02E74516B35DE0BE89592AE847C3611E13CA47A7DC98E1694375D30682A995E644945CA54D5E710BFBC92603DFEF12272622483AAF38E0DEC5C8E90E9F9CFD7BF0938B32A8210BF815E063B0D46EEDFC4A6AA7A265E552B46EF455027D8286FDEF84DAC84808B684F254AE6B6C8772EB0FFD8F859B6266739ED496863F2D9F6D9254D3CA2196ADCABC4B08EA29F523898FF768F8B247694A65004D7537130D3E5B258EF1E8C9C037EEF6605B502818100D80666F5EA76168768B950127E7D98D2A1540FC1E4C44AFFEDF563977A5F940222AE617D6772E97B8D66C0A338DFB688F67453BAC9A9FA1EA0BE8805A7966FFA7B4626F9C46827CBD9C119A2A117E39A6E5637F433E123801E371290508095A406464F38D5AFA05F9672C86285FE94857BC5D8ADD155938BE52CC6A13F2C7BCF02818100D9A1D0802D2747BCFE3825D8BB9C18F0B095C971F5361632ADA5458990EF70F016CFABE5B4E6C5F825D185E2C7A2678273621984DEB7F78A1888615E854D73E2A2C2F566C7167D20F730D36D5E48C569EC424BCE38433187545C0A38A52CE109980143D113A1A33FE7E1BA11870DDBF8C87FB0ED742DAB970B113C181CBE8E5D02818100CB5151D86432E806F926E1F349DF9EE45B9A692EF56D73C3B2C8D63436F08B4D4DD14CB2433EF9DDB242973035874262C9D6E5648190EB681ED170F64360696437C98E184062075672979FA81F258AEBB323F86D5E01125A76CA6BD31E97236D3314FF448CE187E17E8A26025FFEA9E70B148FB2A6E721749B756FA6D20BBFD102818073378C8017E79E731D2CC8BE08F849705D7C79B4CD1CA25710A2CA76016FB442DEE66A1032984AB08C7DFB962D7418AE79520D8294070A946757429B73BF97B4380CDC457875514DB00ACA48F5AE0E29049B918B4AF66583E15DE74B2A44EF9BAAD37E4182DD1A30E42C355492CB1A1A4C075DAAE3273CB960364CFDB4DD5A6D02818001A0FC2814D4371DD3344BD7605E780037C5CC53880A785EA7FA6F52F78DED2F4E56AEBA3EF6C78B7B9F075EB41F17A058CFFFB94C5F7339335416B315CB212967E0E1895EC722ED2EFE45774D09E09AA9C8E4005EBA71328FC1678CC7578274E026DCAAABCFEA83759551706E8AC94CABA16F2026C0A975CE4DEF1DE2BB1CE2"
#define PRIVATE_KEY "30820120300D06092A864886F70D01010105000382010D00308201080282010100B7A5F93E915C1C6E4BBC40467697F90FF3BD6917F8FA247D3338A2565FDB6A0DC957A2C20DBC13ADB37C30A901756B2387EAE3D3D6ED294AFF46FD5FA679AB8857F3326D35FB94913A9098077A4B64EDDE933DA62B72D03D277D94E3DD7B2D987F2E5FEA0DD844B4C4DEFFAF5A606DD5A85E8951BB44DF4468B039770B70C83D8DAECAA72076DAF8BFCF4F4F8508F0D67F84809313822CD7E1D4184F8822DCB5A537C5813EFDE88C3A8805F1C7B6C3561F3548DCA2B2DB41CBCAC3E720D68823BDDD4FE2864FA0D7020BA2C18320864A9B8B4C996066628ED2C46B9915206846C733175BD710541A5614408AADB109934CC1AE1CF55301319CF679952777CC33020111"


struct UniqueID
{
    long hostid;
    char cpu_vendor[13];
    //char alignment[3];
    unsigned int cpu_signature;
    unsigned int cpu_featurebits[3];

    unsigned short int product;
    unsigned short int version;
    unsigned int serial;
    unsigned int features;

    char executable_addr[512];
    byte executable_hash[HASH_ALGO::DIGESTSIZE];

    byte salt[32];
    byte random[32];

    byte hash1[HASH_ALGO::DIGESTSIZE];
    byte hash2[HASH_ALGO::DIGESTSIZE];
};

struct LicenseInfo
{
    UniqueID uid;
    byte license[LI_SIZE];
};

class License
{
public:
    License();
	~License();
    void printUID(UniqueID uid) const;
    string toHex(byte* input, int len) const;
    void fromHex(string input, byte *&output, int& len) const;

    void setUID(UniqueID uid);

    int checkLicense1(byte* license, int size);
    int checkLicense1(string license);
    int checkLicense2();

    string generateLicense1();

private:
    LicenseInfo lInfo;

    void RsaGenerateStringKeys(std::string & publicKeyStr, std::string & privateKeyStr)
    {
        // Pseudo Random Number Generator
        CryptoPP::AutoSeededRandomPool rng;

        // Generate Parameters
        CryptoPP::InvertibleRSAFunction params;
        params.GenerateRandomWithKeySize(rng, 2048);

        CryptoPP::RSA::PrivateKey privateKey(params);
        CryptoPP::RSA::PublicKey publicKey(params);

        CryptoPP::HexEncoder encoder;

        // save public Key
        encoder.Attach( new CryptoPP::StringSink( publicKeyStr ));
        publicKey.Save(encoder);

        // save private Key
        encoder.Attach( new CryptoPP::StringSink( privateKeyStr ));
        privateKey.Save(encoder);
    }

    void RsaSignVector(const std::string & privateKeyStrHex, const std::vector<char> & vec, std::string & sign)
    {
        // Pseudo Random Number Generator
        CryptoPP::AutoSeededRandomPool rng;
        // Generate Parameters
        CryptoPP::InvertibleRSAFunction params;
        params.GenerateRandomWithKeySize(rng, 2048);

        CryptoPP::HexDecoder decoder;
        decoder.Put( (byte*)privateKeyStrHex.c_str(), privateKeyStrHex.size() );
        decoder.MessageEnd();

        CryptoPP::RSA::PrivateKey privateKey; // Private
        privateKey.Load( decoder );


        CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA1>::Signer signer( privateKey );

        size_t length = signer.MaxSignatureLength();
        CryptoPP::SecByteBlock signature( length );

        // Sign message
        signer.SignMessage( rng, (const byte*) vec.at(0), vec.size(), signature );

        sign  = toHex(signature, signature.size());
    }

    bool RsaVerifyVector(const std::string & publicKeyStrHex, const std::string& source, const std::vector<char>& sign)
    {
        CryptoPP::HexDecoder decoder;
        decoder.Put( (byte*)publicKeyStrHex.c_str(), publicKeyStrHex.size() );
        decoder.MessageEnd();

        CryptoPP::RSA::PublicKey publicKey;
        publicKey.Load( decoder );

        // Verifier object
        CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA1>::Verifier verifier( publicKey );

        std::vector<char> rawSignature;
        std::string signStr(sign.at(0), sign.size());
        // FIXME
        //utils::FromHexString(utils::string2wstring(signStr), &rawSignature);
        // Verify
        const char * pData = (const char *)source.at(0);
        return verifier.VerifyMessage( (const byte*) pData, source.size(), (const byte*) rawSignature.at(0), rawSignature.size() );
    }

};