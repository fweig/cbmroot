#include <arpa/inet.h>

#include <stdio.h>

#include <iostream>
#include <string.h>

using namespace std;

#define NUM 4096

int main()
{
	string filename = "/store/cbm_rich_data_2014/flib/1040_cern2014.hld";

	FILE* f = fopen(filename.c_str(), "r");

	unsigned int buff[NUM];
	unsigned int buff2;

	if (f != NULL) {

		fread(&buff, 4, NUM, f);

		for (unsigned int i=0; i<NUM; i++) {
			buff2 = htonl(buff[i]);
			printf ("%08x ", buff2);
			if ((i+1)%8 == 0) printf ("\n");
		}

		fclose(f);
	} else {
		cout << "Error opening file " << filename << endl;
	}

	return 0;
}
