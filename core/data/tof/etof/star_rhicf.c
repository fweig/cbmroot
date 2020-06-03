/********************************************************************************
 *    Copyright (C) 2016-2017 STAR, RHIC, Brookhaven National Laboratory        *
 *                                                                              *
 ********************************************************************************/
// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                             star_rhicf.c                          -----
// -----                  Version of 22.01.2017 by Tonko Ljubicic          -----
// -----              Copied from STAR CVS 22.01.2017 by P.-A.Loizeau      -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>

typedef unsigned int UINT32 ;

#define DAQ_RAW_FORMAT_ORDER    0x04030201
#define DAQ_RAW_FORMAT_VERSION  0x00020003      // 2.30

struct bankHeader {
        char bank_type[8] ;
        UINT32 length ;
        UINT32 bank_id ;
        UINT32 format_ver ;
        UINT32 byte_order ;
        UINT32 format_number ;
        UINT32 token ;
        UINT32 w9 ;
        UINT32 crc ;
} ;

//#define RHICF_FORMAT_NUMBER	0x15112016
#define RHICF_FORMAT_NUMBER	0x24112016

#define RHICF_DATA_PORT		5000


//various hosts
#define LOCALHOST	0x7F000001
#define RHICF		0xAC100C13

static int receiver = LOCALHOST ;

static int b_write(int desc, char *dta, int bytes)
{
	errno = 0 ;
	do {
		int ret = write(desc,dta,bytes) ;

		if(ret == 0) break ;	//EOF
		else if(ret < 0) return -1 ;

		bytes -= ret ;
		dta += ret ;
	} while(bytes) ;


	return 0 ;
}

/*
	trg_word received is packed as:

	trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo

	bytes seems to be ~2664
*/
int star_rhicf_write(unsigned int trg_word, void *dta, int bytes)
{
	static int desc ;
	static int evt ;
	struct bankHeader bh ;
	int bytes_expect = 0 ;


	if(desc<=0) {
		int optval ;
		int ret ;
		struct sockaddr_in s_addr ;

		desc = socket(AF_INET, SOCK_STREAM,0) ;


		memset(&s_addr,0,sizeof(s_addr)) ;
		s_addr.sin_addr.s_addr = htonl(receiver) ;	
		s_addr.sin_port = htons(RHICF_DATA_PORT) ;
		s_addr.sin_family = AF_INET ;

                errno = 0 ;
                ret = connect(desc,(struct sockaddr *)&s_addr,sizeof(s_addr)) ;
                if(ret < 0) {
			fprintf(stderr,"star_rhicf_write: connect to server failed [%s]\n",strerror(errno)) ;
			close(desc) ;
			desc = -1 ;
			return -1 ;
		}

		optval = 1 ;
		setsockopt(desc,SOL_SOCKET,SO_REUSEADDR,(char *)&optval,sizeof(optval)) ;		

		optval = 64*1024*1024 ;
		for(;;) {
			ret = setsockopt(desc,SOL_SOCKET,SO_SNDBUF,(char *)&optval,sizeof(optval)) ;
			if(ret >= 0) break ;
			optval /= 2 ;
		}

		//printf("SNDBUF at end is %d\n",optval) ;

		optval = 0 ;
		int size = sizeof(optval) ;
		ret = getsockopt(desc,SOL_SOCKET,SO_SNDBUF,(char *)&optval,&size) ;

		fprintf(stderr,"star_rhicf_write: connected to 0x%08X (SNDBUF is %d)\n",receiver,optval) ;

		evt = 0 ;	//restart evt counter
	}

	//advance the event counter...
	evt++ ;

	//write stuff
	memcpy(bh.bank_type,"RHICF_dd",8) ;
	bh.length = bytes ;
	bh.bank_id = evt ;	//note hack!
	bh.format_ver = DAQ_RAW_FORMAT_VERSION ;
	bh.byte_order = DAQ_RAW_FORMAT_ORDER ;
	bh.format_number = RHICF_FORMAT_NUMBER ;
	bh.token = trg_word & 0xFFF ;
	bh.w9 = (sizeof(bh)+bytes)/4 ;
	bh.crc = trg_word ;	//note hack!


	bytes_expect = sizeof(bh) + bytes ;

	if(b_write(desc,(char *)&bh,sizeof(bh))<0) {
      close( desc );
      desc = -1;
		goto err_ret ;
	}

	if(b_write(desc,dta,bytes)<0) {
      close( desc );
      desc = -1;
		goto err_ret ;
	}

	return bytes_expect ;

	err_ret:;

	fprintf(stderr,"write error [%s]\n",strerror(errno)) ;

	return -1 ;

}

#ifdef MAIN
int main(int argc, char *argv[])
{
	char dta[5*1024] ;
	int evts = 10 ;
	int i ;

	memset(dta,'A',sizeof(dta)) ;

	switch(argc) {
	case 2 :
		evts = atoi(argv[1]) ;
		break ;
	case 3 :
		evts = atoi(argv[1]) ;
		receiver = RHICF ;
		break ;
	}

	for(i=0;i<evts;i++) {
		star_rhicf_write(0,dta,sizeof(dta)) ;
	}

	return 0 ;
}
#endif
