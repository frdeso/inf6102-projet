#ifndef _BIOME_H_
#define _BIOME_H_

class Biome {
	public:
		Biome(int type, unsigned int length);
		int	GetType() const;
		void SetType(int type);
		unsigned int GetLength() const;
		void SetLength(unsigned int lenght) ;
	private:
		int type_;
		unsigned int length_;
};
#endif //_BIOME_H_
