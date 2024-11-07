生成密钥是命令是：
# 切换到目标目录
cd D:\project\qt\socket\Server\build\Desktop_Qt_6_8_0_MSVC2022_64bit-Release

# 生成私钥文件 privateKey.pem
openssl genpkey -algorithm RSA -out privateKey.pem -aes256 -pass pass:123456

# 生成自签名证书文件 certificate.pem，证书有效期为 365 天
openssl req -new -x509 -key privateKey.pem -out certificate.pem -days 365 -passin pass:123456 -config "D:\lib\openssl\apps\openssl.cnf" -subj "/C=US/ST=State/L=City/O=Organization/CN=127.0.0.1"
