
#include <stdio.h>
#include <stdlib.h>

void usage(char* argv);

int writeline(char *buf,  int byteline, FILE* out);

int writeblock(char *buf, int count, int byteline, FILE* out);


int main(int argc, char** argv)
{
	int linewrited = 0;
	int read_count;
	int buswidth = 8;
	int byteline = buswidth/8;

	FILE* in_file;
	FILE* out_file;

	if((argc < 3) || (argc > 4))
		usage(&argv[0][0]);

	in_file = fopen(&argv[1][0], "rb");
	if (!in_file){
		printf("Unable to open file %s\n", &argv[1][0]);
		return 1;
	}

	out_file = fopen(&argv[2][0], "w");
	if (!out_file){
		printf("Unable to open file %s\n", &argv[2][0]);
		fclose(in_file);
		return 1;
	}

	if(argc == 4){
		buswidth = atoi(&argv[3][0]);
		if((buswidth < 8) || (buswidth > 512))
			usage(&argv[0][0]);
		byteline = buswidth/8;
	}
	printf("Write memh file %d byte(s) on line\n", byteline);

	char *read_buff = (char*)malloc(32 * byteline);


	while((read_count = fread(read_buff, byteline, 32, in_file)) != 0){
		linewrited += writeblock(read_buff, read_count, byteline, out_file);
	}

	free(read_buff);

	fclose(in_file);
	fclose(out_file);

	printf("Recorded %d lines\n", linewrited);

	return 0;
}

void usage(char* argv){
	printf("Usage: %s in-file out-file [buswidth]\n", argv);
	printf("\t buswidth = 8...512, default [buswidth] = 8\n", argv);
	exit(1);
}

int writeline(char *buf, int byteline, FILE* out){
	for(int i = 0; i < byteline; i++){
		fprintf(out, "%02.2X", buf[i]&0xff); 
	}
	fprintf(out, "\n");

	return 0;
}

int writeblock(char *buf, int count, int byteline, FILE* out){
	int i;
	for(i = 0; i < count; i++){
		writeline(buf, byteline, out);
		buf += byteline;
	}
	return i;
}