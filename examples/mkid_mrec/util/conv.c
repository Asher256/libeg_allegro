#include <stdio.h>

int ibuf;
short sbuf;
char buffer[3];
FILE *file;
FILE *filed;

void conv()
{
	fread((void *)&ibuf,sizeof(int),1,file);
	sbuf = (short)ibuf;
	fwrite((void *)&sbuf,sizeof(short),1,filed);
}

int main(void)
{
	file = fopen("world.map", "rb");
	filed = fopen("World.map", "wb");
	if(!file || !filed) return 1;

	// M A P pgx pgy scx scy
	fread(buffer,sizeof(char),3,file);
	fwrite(buffer,sizeof(char),3,filed);

	while(!feof(file)) {
		conv();
	}

	fcloseall();
}
