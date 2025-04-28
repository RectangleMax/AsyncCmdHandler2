#include <stdio.h>
#include <stdlib.h>
// #include <conio.h>
// #include <mem.h>
#include <string.h>
// #include <dir.h>
// #include <dos.h>
// #include <fcntl.h>
// #include "windowpr.h"
// #include <io.h>

#include <tcp.h>

char mass_files[2000][40];
int kolvo_files;

struct ffblk srgblk1;

union {
char pos_ch[4];
unsigned long pos_l;
} position;

//union {
//char len_ch[2];
//unsigned int len_i;
//} len1;

char str_mta[100],str_password[100];

//void yield() { printf("WAITING..."); }
char host[100];
char host1[100];
unsigned int port;
char verbose=0;
int razmer_block=1024;
int dosilka;
//int timer=180;

WINDOW *wnd_main,*wnd_status;

tcp_Socket socket;

char ap16_bin[3],ap16_data[3],ap16_cfg[3],ap16_virt[3];
char ap16_inout[3],ap16_make[3],ap16_tmp[3],ap16_spr[3],ap16_logs[3];
char ap[5]="ap16";


void srg_log(FILE*,char*,int);
void srg_log2(char*,int);
FILE *fseans,*fprd,*fprm,*flast;
struct dosdate_t srgdate;
struct dostime_t srgtime;
int kolvoSaveDaysLogs=5;
int ind_sz;

char str1[600],str2[600],str3[600];

FILE *f1;


unsigned char buf[9000];

main()
{
 longword host1;
 int status;
 int i,len;
 int len12;

 int k,i1,i2,m;
 unsigned long lcount1,lcount2,lcount3,lcount_int;

// �⥭�� 䠩�� ap16.cfg
f1=fopen("c:\\ap16.cfg","r");
if(f1==NULL)
 {
  error_message("�� ���� ������ 䠩� c:\\ap16.cfg");
  sleep(5);
  clear_message();
  exit(-1);
 }

fgets(str1,10,f1);
ap16_bin[0]=str1[0]; ap16_bin[1]=str1[1]; ap16_bin[2]=0;
fgets(str1,10,f1);
ap16_data[0]=str1[0]; ap16_data[1]=str1[1]; ap16_data[2]=0;
fgets(str1,10,f1);
ap16_cfg[0]=str1[0]; ap16_cfg[1]=str1[1]; ap16_cfg[2]=0;
fgets(str1,10,f1);
ap16_virt[0]=str1[0]; ap16_virt[1]=str1[1]; ap16_virt[2]=0;
fclose(f1);

strcpy(ap16_inout,ap16_cfg);
strcpy(ap16_make,ap16_virt);
strcpy(ap16_tmp,ap16_virt);
strcpy(ap16_spr,ap16_cfg);
strcpy(ap16_logs,ap16_cfg);

sprintf(str1,"%s\\ap16\\params.cfg",ap16_cfg);
f1=fopen(str1,"r");
if(f1==NULL)
 {
  error_message("  �� ���� ������ 䠩� params.cfg!!!  ");
  sleep(2);
  clear_message();
  exit(1);
 }
for(i=0;i<9;i++)
 fgets(str1,400,f1);
sscanf(str1,"%s",str_mta);
fgets(str1,400,f1);
sscanf(str1,"%s",str_password);
fclose(f1);


sprintf(str1,"%s\\ap16\\tcpip.cfg",ap16_cfg);
f1=fopen(str1,"r");
if(f1==NULL)
 {
  error_message("  �� ���� ������ 䠩� tcpip.cfg!!!  ");
  sleep(2);
  clear_message();
  exit(1);
 }
fgets(str1,300,f1);
sscanf(str1,"%s",host);
fgets(str1,300,f1);
sscanf(str1,"%u",&port);
fgets(str1,300,f1);
sscanf(str1,"%d",&sock_delay);
fgets(str1,300,f1);
sscanf(str1,"%d",&verbose);
fgets(str1,300,f1);
sscanf(str1,"%d",&dosilka);

fclose(f1);

// ��।������ �६��� �࠭���� ��ୠ�쭮� ���ଠ樨
sprintf(str1,"%s\\ap16\\localprm.cfg",ap16_cfg);
f1=fopen(str1,"r");
if(f1!=NULL)
 {
  fgets(str1,250,f1);
  sscanf(str1,"%d",&kolvoSaveDaysLogs);
  fclose(f1);
 }

// ����⨥ 䠩��� ��ୠ���
_dos_getdate(&srgdate);        // ��� � �६� ᮧ����� ��ୠ���
// ���⪠ �।���� ��ୠ���
if(srgdate.day>=kolvoSaveDaysLogs)
 {
  for(i=1;i<=srgdate.day-kolvoSaveDaysLogs;i++)
   {
    sprintf(str1,"%s\\ap16\\logs\\seans%02d",ap16_logs,i);
    remove(str1);
    sprintf(str1,"%s\\ap16\\logs\\prd%02d",ap16_logs,i);
    remove(str1);
    sprintf(str1,"%s\\ap16\\logs\\prm%02d",ap16_logs,i);
    remove(str1);
   }
  for(i=srgdate.day+1;i<=31;i++)
   {
    sprintf(str1,"%s\\ap16\\logs\\seans%02d",ap16_logs,i);
    remove(str1);
    sprintf(str1,"%s\\ap16\\logs\\prd%02d",ap16_logs,i);
    remove(str1);
    sprintf(str1,"%s\\ap16\\logs\\prm%02d",ap16_logs,i);
    remove(str1);
   }
 }
else
 {
  for(i=srgdate.day+1;i<31-kolvoSaveDaysLogs+srgdate.day;i++)
   {
    sprintf(str1,"%s\\ap16\\logs\\seans%02d",ap16_logs,i);
    remove(str1);
    sprintf(str1,"%s\\ap16\\logs\\prd%02d",ap16_logs,i);
    remove(str1);
    sprintf(str1,"%s\\ap16\\logs\\prm%02d",ap16_logs,i);
    remove(str1);
   }
 }
sprintf(str1,"%s\\ap16\\logs\\seans%02d",ap16_logs,srgdate.day);
fseans=fopen(str1,"a+");
sprintf(str1,"%s\\ap16\\logs\\prd%02d",ap16_logs,srgdate.day);
fprd=fopen(str1,"a+");
sprintf(str1,"%s\\ap16\\logs\\prm%02d",ap16_logs,srgdate.day);
fprm=fopen(str1,"a+");
sprintf(str1,"%s\\ap16\\logs\\last",ap16_logs);
flast=fopen(str1,"w");

srg_log2("**************************************************",0);


wnd_main=establish_window(0,0,22,80);  //64);
set_colors(wnd_main,ALL,BLUE,WHITE,DIM);
set_title(wnd_main,"TCP/IP");
display_window(wnd_main);
wnd_status=establish_window(0,22,3,80);  //76);
set_colors(wnd_status,ALL,AQUA,WHITE,BRIGHT);
set_title(wnd_status,"���ଠ��");
display_window(wnd_status);


 ind_sz=-1;

 sock_init();
 if(verbose)
  {
   wprintf(wnd_main,"������� IP-����: %s\n",inet_ntoa(buf,gethostid()));
   sprintf(str1,"������� IP-����: %s",inet_ntoa(buf,gethostid()));
   srg_log2(str1,0);
  }
 //sock_yield(NULL,yield);
 host1=resolve(host);

 wprintf(wnd_main,"����⨥ TCP ��ᨨ...\n");
 srg_log2("����⨥ TCP ��ᨨ...",1);
 if(!tcp_open(&socket,0,host1,port,NULL))
  {
   srg_log2("Error: �� ���� ������ TCP ����.",1);
   error_message("  �� ���� ������ TCP ����  ");
   sleep(2);
   exit(1);
  }
 wprintf(wnd_main,"TCP ���� �����.\n");
 srg_log2("TCP ���� �����.",1);
 sock_mode(&socket,TCP_MODE_BINARY);
 wprintf(wnd_main,"��⠭������� ᮥ�������... (�������� %d ᥪ)\n",sock_delay);
 sprintf(str1,"��⠭������� ᮥ�������... (�������� %d ᥪ)",sock_delay);
 srg_log2(str1,0);
 sock_wait_established(&socket,sock_delay,NULL,&status);
 wprintf(wnd_main,"���������� ��⠭������.\n");
 srg_log2("���������� ��⠭������.",1);

 wprintf(wnd_status,"\n��⥭�䨪�樨�...");

 sprintf(buf,"%s <%s> %d %d %d",str_mta,str_password,razmer_block,sock_delay,
	dosilka);
 if(verbose)
  {
   wprintf(wnd_main,"%s\n",buf);
   srg_log2(buf,0);
  }
 sock_write(&socket,buf,518);
 sock_flushnext(&socket);

 while(1)
  {
   kbhit();
   //if(!tcp_tick(NULL)) { status=-1; goto sock_err; }
   sock_wait_input(&socket,sock_delay,NULL,&status);
   if((len=sock_dataready(&socket))<2) continue;
   len12=sock_read(&socket,buf,2);
   buf[len12]=0;
   break;
  }
   //len12=sock_read(&socket,buf,len);
   //buf[len12]=0;
   wprintf(wnd_main,"prm %d: %s",len12,buf);
 if(strcmp(buf,"OK")!=0)
  {
   wprintf(wnd_status,"\n�訡�� ��⥭�䨪�樨 !!!");
   srg_log2("�訡�� ��⥭�䨪�樨 !!!",1);
   sleep(3);
   exit(1);
  }
 else
  {
   wprintf(wnd_status,"\n�ᯥ譠� ��⥭�䨪���.");
   wprintf(wnd_main,"�ᯥ譠� ��⥭�䨪���.\n");
  }
	// ��।�� 䠩��� �� ��४�ਨ OUT
	ind_sz=1;
	wprintf(wnd_main,"���� 䠩��� ��� ��।��...\n");
	wprintf(wnd_status,"\n");
	srg_log2("��।�� 䠩���...",1);
	kolvo_files=0;
	sprintf(str1,"%s\\ap16\\out\\out*",ap16_inout);
	i1=findfirst(str1,&srgblk1,0);
	lcount1=0;
	while(!i1)
	 {   // 横� ����� 䠩��� ���� 䠩��� out* � mass_files
	  lcount1=1;
	  strcpy(mass_files[kolvo_files],srgblk1.ff_name);
	  kolvo_files++;
	  i1=findnext(&srgblk1);
	 }
	if(lcount1==0)   // ��� 䠩��� �� ��।���
	 goto met_prm;

	// sortirovka
	i=0; m=0;
	while(i<kolvo_files)
	 {
	  m=i; i1=m;
	  strcpy(str2,mass_files[m]);
	  while(m<kolvo_files)
	   {
	    if(strcmp(str2,mass_files[m])>0)
	     {
	      i1=m;
	      strcpy(str2,mass_files[m]);
	     }
	    m++;
	   }
	  strcpy(mass_files[i1],mass_files[i]);
	  strcpy(mass_files[i],str2);
	  i++;
	 }

	for(m=0;m<kolvo_files;m++)
	 {   // ��।�� �����஢����� 䠩���
	  sprintf(str2,"��।�� 䠩�� %s",mass_files[m]);
	  srg_log2(str2,1);

	  sprintf(str2,"%s\\ap16\\out\\%s",ap16_inout,mass_files[m]);

	  wprintf(wnd_main,"��।�� 䠩�� %s\n",mass_files[m]);

	  f1=fopen(str2,"rb");
	  fseek(f1,0,SEEK_END);
	  lcount1=ftell(f1);
	  fclose(f1);

	  if(lcount1==0)
	   {
	    sprintf(str2,"���� %s ����� �㫥��� ࠧ���!!!",mass_files[m]);
	    wprintf(wnd_main,"%s\n",str2);
	    srg_log2(str2,1);
	    continue;
	   }

	  clear_window(wnd_status);
	  wnd_status->_cr=1;
	  wprintf(wnd_status,"��।��: %15s ࠧ���: %10ld ��।���: %10d",
	     mass_files[m],lcount1,0l);


	  sprintf(buf,"%s %ld",strlwr(mass_files[m]),lcount1);
	  sock_write(&socket,buf,518);
	  sock_flushnext(&socket);

	  lcount2=0;
	  if((i=open(str2,O_RDONLY | O_BINARY))==-1)
	   {
	    srg_log2("�� ���� ������ 䠩� ��� ��।��!",1);
	    error_message(" �� ���� ������ 䠩� ��� ��।��! ");
	    sleep(2);
	    wprintf(wnd_main,"����� ᮥ�������.\n");
	    srg_log2("����� ᮥ�������.",1);
	    sock_close(&socket);
	    sock_wait_closed(&socket,sock_delay,NULL,&status);
	    goto sock_err;
	   }
	  while((k=read(i,buf+4,razmer_block))!=0)
	   {
	    if(k==-1)
	     {
	      srg_log2("�訡�� �⥭�� 䠩��!",1);
	      error_message(" �訡�� �⥭�� 䠩��! ");
	      sleep(2);
	      wprintf(wnd_main,"����� ᮥ�������.\n");
	      srg_log2("����� ᮥ�������.",1);
	      sock_close(&socket);
	      sock_wait_closed(&socket,sock_delay,NULL,&status);
	      goto sock_err;
	     }
	    position.pos_l=lcount2;
	    buf[0]=position.pos_ch[0];
	    buf[1]=position.pos_ch[1];
	    buf[2]=position.pos_ch[2];
	    buf[3]=position.pos_ch[3];
	    //len1.len_i=k;
	    //buf[4]=len1.len_ch[0];
	    //buf[5]=len1.len_ch[1];

	    sock_write(&socket,buf,k+4);
	    sock_flushnext(&socket);      // ���뫠�� ������ + �����
	    if(verbose)
	     {
	      sprintf(str2,"Snd pos=%10ld len=%d",lcount2,k);
	      srg_log2(str2,0);
	      wprintf(wnd_main,"Snd pos=%10ld len=%d\n",lcount2,k);
	     }

	    while(1)              // ����砥� ������ ��� ����� �����
	     {
	      kbhit();
	      //if(!tcp_tick(NULL)) { status=-1; goto sock_err; }
	      sock_wait_input(&socket,sock_delay,NULL,&status);
	      if((len=sock_dataready(&socket))<4) continue;
	      len12=sock_read(&socket,buf,4);
	      buf[len12]=0;
	      break;
	     }
	    position.pos_ch[0]=buf[0];
	    position.pos_ch[1]=buf[1];
	    position.pos_ch[2]=buf[2];
	    position.pos_ch[3]=buf[3];
	    lcount2=position.pos_l;
	    if(verbose)
	     {
	      sprintf(str2,"Rcv ack: next pos=%10ld",lcount2);
	      srg_log2(str2,0);
	      wprintf(wnd_main,"Rcv ack: next pos=%10ld\n",lcount2);
	     }
	    wnd_status->_cr=56;
	    wprintf(wnd_status,"%10ld",lcount2);
	    lseek(i,lcount2,SEEK_SET);
	   }
	  close(i);

	  srg_log2("���� ��।��.",1);
	  wprintf(wnd_main,"���� %s ��।��\n",mass_files[m]);
	  sprintf(str2,"%s\\ap16\\out\\%s",ap16_inout,mass_files[m]);
	  remove(str2);
	 }  // ����� ��।�� �����஢����� 䠩���

met_prm:
	//if(verbose)
	// {
	//  wprintf(wnd_main,"���뫠� �������� ����砭��: end 0\n");
	// }
	sprintf(buf,"end 0");
	sock_write(&socket,buf,518);
	sock_flushnext(&socket);      // ���뫠�� �������� ����砭�� ��
	wprintf(wnd_main,"�����襭�� ��।�� 䠩���.\n");

	ind_sz=0;
	srg_log2("���樠������ �ਥ�� 䠩���.",1);
	wprintf(wnd_main,"���樠������ �ਥ�� 䠩���.\n");
	// �ਥ� 䠩���
	while(1)
	 {
	  while(1)    // �ਥ� ���������� 䠩���
	   {
	    kbhit();
	    sock_wait_input(&socket,sock_delay,NULL,&status);
	    if((len=sock_dataready(&socket))<518) continue;
	    len12=sock_read(&socket,buf,518);
	    buf[len12]=0;
	    break;
	   }
	  sscanf(buf,"%s %ld",str1,&lcount1);
	  if(strcmp(str1,"end")==0)
	   {
	    srg_log2("����� �ਥ�� 䠩��� �����襭.",1);
	    wprintf(wnd_main,"����� �ਥ�� 䠩��� �����襭.\n");
	    goto met_end;
	   }

	  sprintf(str2,"�ਭ������� 䠩� %s ࠧ��஬ %ld",str1,lcount1);
	  srg_log2(str2,1);
	  wprintf(wnd_main,"%s\n",str2);
	  clear_window(wnd_status);
	  wnd_status->_cr=0;
	  wprintf(wnd_status,"�ਥ�: %15s ࠧ���: %10ld �ਭ��: %10d",
	     str1,lcount1,0l);

	  sprintf(str2,"%s\\ap16\\in\\%s",ap16_inout,str1);
	  if(dosilka==0)
	   {   // �᫨ ��� ���뫪� ���ଠ樨
	    lcount3=0;
	    if((i=_creat(str2,FA_ARCH))<0)
	     i=_open(str2,O_WRONLY|O_TRUNC|O_BINARY);
	    else
	     { _close(i); i=_open(str2,O_WRONLY|O_TRUNC|O_BINARY); }
	   }
	  else
	   {   // ���� ���뫪� ���ଠ樨
	    if(access(str2,0)==0)
	     {  // 䠩� �������
	      i=_open(str2,O_RDWR|O_BINARY);
	      if(i==-1)
	       {
		sprintf(str1,"�� ���� ������ %s ��� �ਥ��!",str2);
		srg_log2(str1,1);
		error_message(str1);
		sleep(2);
		goto met_end;
	       }
	      lcount3=lseek(i,0,SEEK_END);
	     }
	    else
	     {
	      lcount3=0;
	      if((i=_creat(str2,FA_ARCH))<0)
	       i=_open(str2,O_WRONLY|O_TRUNC|O_BINARY);
	      else
	       { _close(i); i=_open(str2,O_WRONLY|O_TRUNC|O_BINARY); }
	     }
	   }

	  if(i==-1)
	   {
	    sprintf(str1,"�� ���� ������ %s ��� �ਥ��!",str2);
	    srg_log2(str1,1);
	    error_message(str1);
	    sleep(2);
	    goto met_end;
	   }

	  lcount2=0;
	  while(lcount1>lcount2)
	   {
	    if((lcount1-lcount2)<razmer_block)
	     lcount_int=lcount1-lcount2;
	    else
	     lcount_int=razmer_block;

	    if(verbose)
	     wprintf(wnd_main,"Wait for pos=%d len=%d\n",lcount2,lcount_int);
	    while(1)    // �ਥ� ����� ������
	     {
	      kbhit();
	      sock_wait_input(&socket,sock_delay,NULL,&status);
	      if((len=sock_dataready(&socket))<lcount_int+4) continue;
	      len12=sock_read(&socket,buf,lcount_int+4);
	      buf[len12]=0;
	      break;
	     }

	    position.pos_ch[0]=buf[0];
	    position.pos_ch[1]=buf[1];
	    position.pos_ch[2]=buf[2];
	    position.pos_ch[3]=buf[3];
	    if(verbose)
	     {
	      sprintf(str2,"Rcv data: pos=%10ld len=%d",position.pos_l,
		 lcount_int);
	      srg_log2(str2,0);
	      wprintf(wnd_main,"Rcv data: pos=%10ld len=%d\n",position.pos_l,
		 lcount_int);
	     }

	    if((dosilka) && (lcount3) && (lcount2==0))
	     {
	      wprintf(wnd_main,"���뫪�: pos=%ld\n",lcount3);
	      srg_log2(str2,0);
	      lcount2=lcount3;
	      position.pos_l=lcount2;
	      buf[0]=position.pos_ch[0];
	      buf[1]=position.pos_ch[1];
	      buf[2]=position.pos_ch[2];
	      buf[3]=position.pos_ch[3];
	      sock_write(&socket,buf,4);   // ���뫠�� ������ ��� ���뫪�
	      sock_flushnext(&socket);
	      continue;
	     }
	    else
	     {
	      if(lcount2!=position.pos_l)
	       {
		sprintf(str2,"Error: current pos %ld <> prm %ld",
		  lcount2,position.pos_l);
		srg_log2(str2,0);
		wprintf(wnd_main,"Error: current pos %ld <> prm %ld\n",
		  lcount2,position.pos_l);

		position.pos_l=lcount2;
		buf[0]=position.pos_ch[0];
		buf[1]=position.pos_ch[1];
		buf[2]=position.pos_ch[2];
		buf[3]=position.pos_ch[3];
		sock_write(&socket,buf,4);   // ���뫠�� ������ ��� �����
		sock_flushnext(&socket);
		continue;
	       }
	     }

	    if((k=_write(i,buf+4,lcount_int))!=lcount_int)
	     {
	      srg_log2("�訡�� ����� � 䠩�!",1);
	      error_message(" �訡�� ����� � 䠩�! ");
	      sleep(2);
	      goto met_end;
	     }
	    lcount2+=lcount_int;
	    position.pos_l=lcount2;
	    buf[0]=position.pos_ch[0];
	    buf[1]=position.pos_ch[1];
	    buf[2]=position.pos_ch[2];
	    buf[3]=position.pos_ch[3];
	    sock_write(&socket,buf,4);   // ���뫠�� ������ next
	    sock_flushnext(&socket);

	    if(verbose)
	     {
	      sprintf(str2,"Snd next pos=%ld",lcount2);
	      srg_log2(str2,0);
	      wprintf(wnd_main,"Snd next pos=%ld\n",lcount2);
	     }
	    wnd_status->_cr=52;
	    wprintf(wnd_status,"%10ld",lcount2);
	   }
	  _close(i);
	  sprintf(str2,"�ਭ�� 䠩� %s",str1);
	  srg_log2(str2,1);
	  wprintf(wnd_main,"�ਭ�� 䠩� %s\n",str1);
	  sprintf(str2,"%s\\ap16\\in1\\%s",ap16_inout,str1);
	  remove(str2);
	  sprintf(str3,"%s\\ap16\\in\\%s",ap16_inout,str1);
	  rename(str3,str2);
	 }  // end of main cykl

met_end: ;
 wprintf(wnd_main,"����� ᮥ�������.\n");
 srg_log2("����� ᮥ�������.",1);
 sock_close(&socket);
 sock_wait_closed(&socket,sock_delay,NULL,&status);

sock_err: ;
 if(status==1)
  {
   wprintf(wnd_main,"��ଠ�쭮� �����襭�� �ணࠬ��.\n");
   srg_log2("��ଠ�쭮� �����襭�� �ணࠬ��.",1);
  }
 else
  {
   sprintf(str1,"Error: %s",sockerr(&socket));
   srg_log2(str1,1);
   error_message(str1);
   sleep(3);
  }
 sleep(1);
 exit(0);
}




void srg_log(FILE *f,char* str_log,int ind_time)
{

 if(ind_time)
  {
   _dos_gettime(&srgtime);
   fprintf(f,"%02d:%02d:%02d ",srgtime.hour,srgtime.minute,srgtime.second);
  }
 else
  fprintf(f,"         ");

 fprintf(f,"%s\n",str_log);
}



void srg_log2(char *str_log,int ind_time)
{
 srg_log(fseans,str_log,ind_time);
 srg_log(flast,str_log,ind_time);

 if(ind_sz==1)
  srg_log(fprd,str_log,ind_time);
 else
  if(ind_sz==0)
   srg_log(fprm,str_log,ind_time);
}
