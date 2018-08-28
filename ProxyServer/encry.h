#ifndef __ENCRY_H__
#define __ENCRY_H__

//异或处理数据
static inline char* Xor(char* buf, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		buf[i] ^= 1;
	}
}

//解密
static inline void Decrypt(char* buf, size_t len)
{
	Xor(buf, len);
}

//加密
static inline void Encry(char* buf, size_t len)
{
	Xor(buf, len);
}

#endif //__ENCRY_H__
