#ifndef TIGERAPI_TEST_SIGN_HPP
#define TIGERAPI_TEST_SIGN_HPP

#include <iostream>
#include "../base64.hpp"
#include "../common/rsa_sign.hpp"
#include "../utils.h"


//void test_sign();

void test_sign() {
    unsigned char plainText[2048 /
                            8] = "biz_content={\"lang\":\"en_US\",\"market\":\"ALL\"}&charset=UTF-8&method=market_state&sign_type=RSA&tiger_id=1&timestamp=2019-06-06 13:51:54&version=2.0"; //key length : 2048

    unsigned char publicKey1[] = "-----BEGIN PUBLIC KEY-----\n"\
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"\
        "ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"\
        "vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"\
        "fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"\
        "i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"\
        "PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"\
        "wQIDAQAB\n"\
        "-----END PUBLIC KEY-----\n";

    unsigned char privateKey1[] = "-----BEGIN RSA PRIVATE KEY-----\n"\
        "MIIEowIBAAKCAQEAy8Dbv8prpJ/0kKhlGeJYozo2t60EG8L0561g13R29LvMR5hy\n"\
        "vGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+vw1HocOAZtWK0z3r26uA8kQYOKX9\n"\
        "Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQApfc9jB9nTzphOgM4JiEYvlV8FLhg9\n"\
        "yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68i6T4nNq7NWC+UNVjQHxNQMQMzU6l\n"\
        "WCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoVPpY72+eVthKzpMeyHkBn7ciumk5q\n"\
        "gLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUywQIDAQABAoIBADhg1u1Mv1hAAlX8\n"\
        "omz1Gn2f4AAW2aos2cM5UDCNw1SYmj+9SRIkaxjRsE/C4o9sw1oxrg1/z6kajV0e\n"\
        "N/t008FdlVKHXAIYWF93JMoVvIpMmT8jft6AN/y3NMpivgt2inmmEJZYNioFJKZG\n"\
        "X+/vKYvsVISZm2fw8NfnKvAQK55yu+GRWBZGOeS9K+LbYvOwcrjKhHz66m4bedKd\n"\
        "gVAix6NE5iwmjNXktSQlJMCjbtdNXg/xo1/G4kG2p/MO1HLcKfe1N5FgBiXj3Qjl\n"\
        "vgvjJZkh1as2KTgaPOBqZaP03738VnYg23ISyvfT/teArVGtxrmFP7939EvJFKpF\n"\
        "1wTxuDkCgYEA7t0DR37zt+dEJy+5vm7zSmN97VenwQJFWMiulkHGa0yU3lLasxxu\n"\
        "m0oUtndIjenIvSx6t3Y+agK2F3EPbb0AZ5wZ1p1IXs4vktgeQwSSBdqcM8LZFDvZ\n"\
        "uPboQnJoRdIkd62XnP5ekIEIBAfOp8v2wFpSfE7nNH2u4CpAXNSF9HsCgYEA2l8D\n"\
        "JrDE5m9Kkn+J4l+AdGfeBL1igPF3DnuPoV67BpgiaAgI4h25UJzXiDKKoa706S0D\n"\
        "4XB74zOLX11MaGPMIdhlG+SgeQfNoC5lE4ZWXNyESJH1SVgRGT9nBC2vtL6bxCVV\n"\
        "WBkTeC5D6c/QXcai6yw6OYyNNdp0uznKURe1xvMCgYBVYYcEjWqMuAvyferFGV+5\n"\
        "nWqr5gM+yJMFM2bEqupD/HHSLoeiMm2O8KIKvwSeRYzNohKTdZ7FwgZYxr8fGMoG\n"\
        "PxQ1VK9DxCvZL4tRpVaU5Rmknud9hg9DQG6xIbgIDR+f79sb8QjYWmcFGc1SyWOA\n"\
        "SkjlykZ2yt4xnqi3BfiD9QKBgGqLgRYXmXp1QoVIBRaWUi55nzHg1XbkWZqPXvz1\n"\
        "I3uMLv1jLjJlHk3euKqTPmC05HoApKwSHeA0/gOBmg404xyAYJTDcCidTg6hlF96\n"\
        "ZBja3xApZuxqM62F6dV4FQqzFX0WWhWp5n301N33r0qR6FumMKJzmVJ1TA8tmzEF\n"\
        "yINRAoGBAJqioYs8rK6eXzA8ywYLjqTLu/yQSLBn/4ta36K8DyCoLNlNxSuox+A5\n"\
        "w6z2vEfRVQDq4Hm4vBzjdi3QfYLNkTiTqLcvgWZ+eX44ogXtdTDO7c+GeMKWz4XX\n"\
        "uJSUVL5+CVjKLjZEJ6Qc2WZLl94xSwL71E41H4YciVnSCQxVc4Jw\n"\
        "-----END RSA PRIVATE KEY-----\n";

    //tigerid =1
    unsigned char publicKey[] = "-----BEGIN PUBLIC KEY-----\n"\
        "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC1amZa5YsGTklry7DAsUBOwXJC\n"\
        "grsZZtB21PImw/yLmrbqRfsS3vawvMigLWcCwIDnHa+hpdpeze0eHIwbZzJzUDGv\n"\
        "RALYK9t3D8pwPVxpwX1OF8RfHCM7YQvSOvPPnHHuVQvKaR7NNm1/WmvGXC9kVJdk\n"\
        "YQ7kCmh52siFoy1MLQIDAQAB\n"\
        "-----END PUBLIC KEY-----\n";

    unsigned char privateKey2[] = "-----BEGIN PRIVATE KEY-----\n"\
        "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBALVqZlrliwZOSWvLsMCxQE7BckKCuxlm0HbU8ibD/IuatupF+xLe9rC8yKAtZwLAgOcdr6Gl2l7N7R4cjBtnMnNQMa9EAtgr23cPynA9XGnBfU4XxF8cIzthC9I688+cce5VC8ppHs02bX9aa8ZcL2RUl2RhDuQKaHnayIWjLUwtAgMBAAECgYBVptyYgdO3sHKdwFe9QnKYPGET4eEwRwiZckS55MJxryQ5S70U1cwm+ZrHdWgzNmwlsjBf44xMQnF3TAciUQDSAQKMPAUrhDjar6m07hAAElBVILoj5063Y4hjby69PeuR6c47wpEgO3gXnY8V3vVl0HVppeLE5E5GrUx1IuLwSQJBAOK+IAH4vlH8dXHDG+pl8nnfQ60V3kiKlkL1hmVjUK1dYKa7z0srtRSUS41fIaw7HLPmMuoW41GyJ7kALbe18Q8CQQDM0wJrxrElZ89kgfi0attnUXcjjUFB2CnbMGZHsDJQwEOSWLzo7OYntBQIkD88nErjf8hhRabg5TlD5K9cKvcDAkEAlMGPVjvPARLUKlKK2fkQX4xuZfvW3ViuxDc4eQPzRgnhAwJsOTDvmBTCjHlBKjGN3yjDdniVol9zWBgZ5lDjWQJBAIKJDCbxxx4GrdbpOj6JR3DuK1aeBHPZNru5ecXAViC+l5HiOercZ8Xv70CR5atUFCou2t6bTEJR9A//436C508CQEUtu3vz8ylAYQNzuPK78oTkynjAwwNBDByH1Uu+wBnBYvKwHqtlamjeMh+n3NntzfM5gY0xYBmO0yY2REnwI4c=\n"\
        "-----END PRIVATE KEY-----\n";

    unsigned char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n"\
        "MIICXQIBAAKBgQC1amZa5YsGTklry7DAsUBOwXJCgrsZZtB21PImw/yLmrbqRfsS\n"\
        "3vawvMigLWcCwIDnHa+hpdpeze0eHIwbZzJzUDGvRALYK9t3D8pwPVxpwX1OF8Rf\n"\
        "HCM7YQvSOvPPnHHuVQvKaR7NNm1/WmvGXC9kVJdkYQ7kCmh52siFoy1MLQIDAQAB\n"\
        "AoGAVabcmIHTt7ByncBXvUJymDxhE+HhMEcImXJEueTCca8kOUu9FNXMJvmax3Vo\n"\
        "MzZsJbIwX+OMTEJxd0wHIlEA0gECjDwFK4Q42q+ptO4QABJQVSC6I+dOt2OIY28u\n"\
        "vT3rkenOO8KRIDt4F52PFd71ZdB1aaXixORORq1MdSLi8EkCQQDiviAB+L5R/HVx\n"\
        "wxvqZfJ530OtFd5IipZC9YZlY1CtXWCmu89LK7UUlEuNXyGsOxyz5jLqFuNRsie5\n"\
        "AC23tfEPAkEAzNMCa8axJWfPZIH4tGrbZ1F3I41BQdgp2zBmR7AyUMBDkli86Ozm\n"\
        "J7QUCJA/PJxK43/IYUWm4OU5Q+SvXCr3AwJBAJTBj1Y7zwES1CpSitn5EF+MbmX7\n"\
        "1t1YrsQ3OHkD80YJ4QMCbDkw75gUwox5QSoxjd8ow3Z4laJfc1gYGeZQ41kCQQCC\n"\
        "iQwm8cceBq3W6To+iUdw7itWngRz2Ta7uXnFwFYgvpeR4jnq3GfF7+9AkeWrVBQq\n"\
        "Ltrem0xCUfQP/+N+gudPAkBFLbt78/MpQGEDc7jyu/KE5Mp4wMMDQQwch9VLvsAZ\n"\
        "wWLysB6rZWpo3jIfp9zZ7c3zOYGNMWAZjtMmNkRJ8COH\n"\
        "-----END RSA PRIVATE KEY-----\n";

    unsigned char encrypted[8196 * 16] = {};
    unsigned char decrypted[4098] = {};
    get_sign(plainText, privateKey);


//        int decrypted_length = sha1RSASign.decrypt(encrypted, encrypted_length, publicKey, decrypted);
//        if (decrypted_length == -1) {
//            sha1RSASign.printLastError("Public Decrypt failed");
//            exit(0);
//        }
//        printf("Decrypted Text =%s\n", decrypted);
//        printf("Decrypted Length =%d\n", decrypted_length);
}

#endif
