

#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "smdi.h"
#include "smdistatic.h"

#include "popt-1.6.4/popt.h"


#define VERSION_MAJOR 0
#define VERSION_MINOR 1



char *arg_sample_name = "unnamed";
char *arg_filename = "unnamed.wav";
int arg_sample_number = -1;
int arg_scsi_id = -1;
int arg_host_adapter = -1;
int arg_info = 0;
int arg_query = 0;
int arg_send = 0;
int arg_receive = 0;
int arg_use_sampler_name = 0;
int arg_debug = 0;
int arg_version = 0;



void smdi_info(int host_adapter, int scsi_id)
{
  int result;
  int sample_number;
  SCSI_DevInfo dev_info;
  
  printf("Trying to get device info for device on adapter %d with scsi id %d... ", host_adapter, scsi_id);
  dev_info.dwStructSize=sizeof(struct SCSI_DevInfo);
  SMDI_GetDeviceInfo(host_adapter, scsi_id, &dev_info);
  printf("done.\n");
  printf("Device name: %s\n", dev_info.cName);
  printf("Manufacturer's name: %s\n", dev_info.cManufacturer);
  printf("SMDI compatible: %s\n", dev_info.bSMDI?"Yes":"No");
  return;
}

int smdi_query_header(int host_adapter, int scsi_id, int position)
{
  int result;
  SMDI_SampleHeader sample_header;

  printf("Getting sample header for sample number %d...", position);
  sample_header.dwStructSize=sizeof(struct SMDI_SampleHeader);
  result = SMDI_SampleHeaderRequest(host_adapter, scsi_id, position, &sample_header);
  printf(" done\n");
  if (result == SMDIM_SAMPLEHEADER)
	{
	  printf("Name: %s\n", sample_header.cName);
	  printf("Bits per word: %d\n", sample_header.BitsPerWord);
	  printf("Number of channels: %d\n", sample_header.NumberOfChannels);
	  printf("Period: %d\n", sample_header.dwPeriod);
	  printf("Loop control: %d\n", sample_header.LoopControl);
	  printf("Size: %d\n", sample_header.dwLength);
	  printf("Loop start: %d\n", sample_header.dwLoopStart);
	  printf("Loop end: %d\n", sample_header.dwLoopEnd);
	  printf("Pitch: %d\n", sample_header.wPitch);
	  printf("Pitch fraction: %d\n", sample_header.wPitchFraction);
	  printf("Data offset: %d\n", sample_header.dwDataOffset);
	}
  else
	{
	  if (result==SMDIE_OUTOFRANGE)
		printf("Sample out of range\n");
	  else if (result==SMDIE_NOSAMPLE)
		printf("No sample\n");
	  else
		printf("Unknown error (%d)\nAre you sure that you have an SMDI-capable sampler on your SCSI chain?\n", result);
	}
}

void display_status(SMDI_FileTransmissionInfo *i, int a)
{
  int j;
  unsigned char ch = i->lpTransmissionInfo->lpSampleHeader->NumberOfChannels;  
  unsigned char bpw = i->lpTransmissionInfo->lpSampleHeader->BitsPerWord;
  int length = i->lpTransmissionInfo->lpSampleHeader->dwLength;
  int packetSize = i->lpTransmissionInfo->dwPacketSize;
  int totalPacketCount = length*(ch*bpw/8)/packetSize;
  int packetNumber = i->lpTransmissionInfo->dwTransmittedPackets;
  int pixels = 24;
  float fractionDone = packetNumber/(float)totalPacketCount;
  int pixelsDone = (int)(fractionDone*(float)pixels);

  printf("\rTransfering packet %04d/%04d of size %06d bytes [", 
		 packetNumber, totalPacketCount, packetSize);
  for (j=0; j<pixelsDone; j++)
	printf("*");
  for (; j<pixels; j++)
	printf(" ");
  printf("]");
  fflush(stdout);

  /*
  if (i->lpTransmissionInfo->dwTransmittedPackets<=totalPacketCount)
	printf("Packet number: %d of %d\n", packetNumber, totalPacketCount);
  */
}

int smdi_receive(int host_adapter, int scsi_id, int position, char *filename, int use_sampler_name)
{
  char namebuffer[1024]="\0";
  int result;
  int sample_number;
  SMDI_SampleHeader sample_header;
  SMDI_FileTransfer file_transfer;
  
  sample_header.dwStructSize=sizeof(struct SMDI_SampleHeader);
  result = SMDI_SampleHeaderRequest(host_adapter, scsi_id, position, &sample_header);

  if (result == SMDIM_SAMPLEHEADER)
	{

	}
  else
	{
	  if (result==SMDIE_OUTOFRANGE)
		printf("Sample out of range\n");
	  else if (result==SMDIE_NOSAMPLE)
		printf("No sample\n");
	  else
		printf("Unknown error (%d)\nAre you sure that you have an SMDI-capable sampler on your SCSI chain?\n", result);
	}

  file_transfer.dwStructSize=sizeof(struct SMDI_FileTransfer);
  file_transfer.HA_ID=host_adapter;
  file_transfer.SCSI_ID=scsi_id;
  file_transfer.dwSampleNumber=position;
  file_transfer.lpFileName=filename;
  file_transfer.dwFileType=FT_WAV;
  file_transfer.lpCallback=display_status;
  file_transfer.bAsync=0;

  if (use_sampler_name)
	{
	  strcpy(namebuffer, sample_header.cName);
	  strcat(namebuffer, ".wav");
	  file_transfer.lpFileName=namebuffer;
	}

  result = SMDI_ReceiveFile(&file_transfer);
  if (result==SMDIM_ENDOFPROCEDURE)
	{
	  printf("\nSuccess!\n");
	}
  else
	{
	  printf("\nFailure.\n");
	}
}



int smdi_send(int host_adapter, int scsi_id, int position, char *filename, char* samplename, int use_sampler_name)
{
  char namebuffer[1024]="\0";
  char *namebuffer_ptr;
  char *namebuffer_ptr2;
  int result;
  int sample_number;
  SMDI_FileTransfer file_transfer;

  file_transfer.dwStructSize=sizeof(struct SMDI_FileTransfer);
  file_transfer.HA_ID=host_adapter;
  file_transfer.SCSI_ID=scsi_id;
  file_transfer.dwSampleNumber=position;
  file_transfer.lpFileName=filename;
  file_transfer.lpSampleName=samplename;
  file_transfer.lpCallback=display_status;
  file_transfer.dwUserData=3;
  file_transfer.bAsync=0;
  file_transfer.lpReturnValue=NULL;

  if (use_sampler_name)
	{
	  int l;
	  strcpy(namebuffer, filename);
	  l=strlen(namebuffer);
	  if (namebuffer[l-4]=='.' && namebuffer[l-3]=='w' && 
		  namebuffer[l-2]=='a' && namebuffer[l-1]=='v')
		{
		  namebuffer[l-4]='\0';
		}
	  namebuffer_ptr = namebuffer;
	  namebuffer_ptr2 = basename((char *)namebuffer_ptr);
	  file_transfer.lpSampleName=namebuffer_ptr2;
	}  

  result = SMDI_SendFile(&file_transfer);
  if (result==SMDIM_ENDOFPROCEDURE)
	{
	  printf("\nSuccess!\n");
	}
  else if (result==FE_OPENERROR)
	{
	  printf("\nCould not find the specified file.\n");
	}
  else if (result==FE_UNKNOWNFORMAT)	
	{
	  printf("Could not understand the file format.\n");
	  printf("I can only handle _uncompressed_ wav-files. Sorry.\n");
	}
  else
	{
	  printf("\nFailure.\n");
	}
}



int main (int argc, const char **argv)
{
  int command=0;

  poptContext optCon;   /* context for parsing command-line options */

  struct poptOption commandOptionsTable[] = {
	{ "info", 0, POPT_ARG_NONE, &arg_info, 0,
	  "Get info about the sampler connected.", NULL },
	{ "query", 0, POPT_ARG_NONE, &arg_query, 0,
	  "Query a sample in the sampler by giving the sample number.", NULL },
	{ "send", 0, POPT_ARG_NONE, &arg_send, 0,
	  "Send a sample to the sampler.", NULL },
	{ "receive", 0, POPT_ARG_NONE, &arg_receive, 0,
	  "Receive a sample from the sampler.", NULL },
	{ NULL, 0, 0, NULL, 0, NULL, NULL }
  };
  struct poptOption settingsOptionsTable[] = {
	{ "host_adapter", 'h', POPT_ARG_INT, &arg_host_adapter, 0,
	  "SCSI host adapter (/dev/sg<integer>).", "<integer>" },
	{ "scsi_id", 'i', POPT_ARG_INT, &arg_scsi_id, 0,
	  "SCSI ID of sampler.", "<integer>" },
	{ "samplename", 's', POPT_ARG_STRING, &arg_sample_name, 0,
	  "Name of sample on the sampler. Used when sending only. Default is \"unnamed\".", "<string>" },
	{ "filename", 'f', POPT_ARG_STRING, &arg_filename, 0,
	  "Name of sample on the host computer. Used when sending and receiving. Default is \"unnamed.wav\".", "<string>" },
	{ "samplenumber", 'n', POPT_ARG_INT, &arg_sample_number, 0,
	  "Number of sample on the sampler to use.", "<integer>" },
	{ "usesamplername", 'u', POPT_ARG_NONE, &arg_use_sampler_name, 0,
	  "Use the name of the sample from the sampler as filename on the host computer.", "<integer>" },
	{ NULL, 0, 0, NULL, 0, NULL, NULL }
  };
  struct poptOption additionalOptionsTable[] = {
	{ "debug", 'd', POPT_ARG_NONE, &arg_debug, 0,
	  "Debug. Well... You know...", NULL },
	{ "version", 'v', POPT_ARG_NONE, &arg_version, 0,
	  "Get program version", NULL },
	{ NULL, 0, 0, NULL, 0, NULL, NULL }
  };

  struct poptOption optionsTable[] = {
	{ NULL, '\0', POPT_ARG_INCLUDE_TABLE,  NULL, 0,
	  "Commands", NULL },
	{ NULL, '\0', POPT_ARG_INCLUDE_TABLE,  NULL, 0,
	  "Settings", NULL },
	{ NULL, '\0', POPT_ARG_INCLUDE_TABLE,  NULL, 0,
	  "Additional options", NULL },
	POPT_AUTOHELP
	{ NULL, 0, 0, NULL, 0, NULL, NULL }
  };

  printf("esmdi - by Erik Jansson 2002, erik@hectronic.se\n");  
  printf("Based on opensmdi by Christian Nowak.\n");

  optionsTable[0].arg = commandOptionsTable;
  optionsTable[1].arg = settingsOptionsTable;
  optionsTable[2].arg = additionalOptionsTable;
  optCon = poptGetContext("Global", argc, argv, optionsTable, 0);
  poptGetNextOpt(optCon);
  optCon = poptFreeContext(optCon);
  
  if (arg_version)
	{
	  printf("Version: %d.%d (Built: %s, %s)\n", VERSION_MAJOR, VERSION_MINOR, __DATE__, __TIME__);
	  exit(0);
	}

  if (arg_debug)
	{
	  printf("Arguments given:\n");
	  printf("  Info: %d\n", arg_info);
	  printf("  Query: %d\n", arg_query);
	  printf("  Send: %d\n", arg_send);
	  printf("  Receive: %d\n", arg_receive);
	  printf("  Host adapter: %d\n", arg_host_adapter);
	  printf("  SCSI ID: %d\n", arg_scsi_id);
	  printf("  Sample name: %s\n", arg_sample_name);
	  printf("  File name: %s\n", arg_filename);
	  printf("\n");
	}
  command=arg_info+(arg_query<<1)+(arg_send<<2)+(arg_receive<<3);

  if (arg_debug)
	printf("Command: %d\n", command);

  if (arg_info+arg_query+arg_send+arg_receive!=1)
	{
	  printf("Please provide at least and at most one command\nUse the --help option to learn about the commands.\n");
	  exit(0);
	}
  if (arg_host_adapter<0 || arg_host_adapter>16)
	{
	  printf("Please provide a SCSI host adapter (between 0 and 16)\n");
	  exit(0);
	}
  if (arg_scsi_id<0 || arg_scsi_id>7)
	{
	  printf("Please provide a SCSI id (between 0 and 7)\n");
	  exit(0);
	}

  
  
  switch (command)
	{

	case 1:
	  printf("Command: Info\n");
	  printf("");
	  smdi_info(arg_host_adapter, arg_scsi_id);
	  break;

	case 2:
	  printf("Command: Query\n");
	  if (arg_sample_number<0 || arg_sample_number>255)
		{
		  printf("Please provide a sample number to query.\n");
		  exit (0);
		}		
	  smdi_query_header(arg_host_adapter, arg_scsi_id, arg_sample_number);
	  break;

	case 4:
	  printf("Command: Send\n");
	  if (arg_sample_number<0 || arg_sample_number>255)
		{
		  printf("Please provide a sample number to send.\n");
		  exit (0);
		}		
	  smdi_send(arg_host_adapter, arg_scsi_id, arg_sample_number, arg_filename, arg_sample_name, arg_use_sampler_name);
	  break;

	case 8:
	  printf("Command: Receive\n");
	  if (arg_sample_number<0 || arg_sample_number>255)
		{
		  printf("Please provide a sample number to receive.\n");
		  exit (0);
		}		
	  smdi_receive(arg_host_adapter, arg_scsi_id, arg_sample_number, arg_filename, arg_use_sampler_name);
	  
	  break;

	default:
	  printf("Unknown command. Sorry.\n");
	  break;
	  
	}



  return 0;
}
