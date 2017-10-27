#ifndef ARMVirtualBusFederate_H_
#define ARMVirtualBusFederate_H_

#include "FederateAmbassador.h"
#include "RTI.hh"

#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>

#define READY_TO_RUN "ReadyToRun"
#define NUM_ELEMENTS 700000

using namespace std;

class VirtualBusFederate
{
	public:
		RTI::RTIambassador *rtiamb;
		FederateAmbassador      *fedamb;
		char federateName[12];

		// fom handles //
		RTI::ObjectHandle      	    oHandle;		
		RTI::ObjectClassHandle      aHandle;
		RTI::AttributeHandle        srcHandle;
		RTI::AttributeHandle        addrHandle;
		RTI::AttributeHandle        sizeHandle;
		RTI::AttributeHandle        data1Handle;
		RTI::AttributeHandle        data2Handle;
		RTI::AttributeHandle        data3Handle;		
		RTI::AttributeHandle        data4Handle;
		RTI::AttributeHandle        data5Handle;
		RTI::AttributeHandle        data6Handle;
		RTI::AttributeHandle        data7Handle;
		RTI::AttributeHandle        data8Handle;
		RTI::AttributeHandle        dataFrameHandle;
		
		

		// public methods //
		VirtualBusFederate();
		virtual ~VirtualBusFederate();
		void runFederate( char* federateName );
		void loop(int iteracoes);
		void finalize();


		void writeData(string src, string addr, string size, string data1, string data2, string data3, string data4, string data5,string data6, string data7, string data8, unsigned char* dataFrame);
		
		bool readData(string& src, string& addr, string& size, string& data1, string& data2, string& data3, string& data4, string& data5, string& data6, string& data7, string& data8, unsigned char* dataFrame);
		
		bool hasReceivedData();
		
		void advanceTime( double timestep );

	private:

		bool receivedData;
		std::ofstream log;
		//Mat image;
		double dif_time_frame_receive;
		clock_t start, capture, receive;
		double difTime;
		//Vaŕiáveis compartilhadas do HLA
		/**
			Para economia de esforço, os nomes variáveis que são utilizadas no HLA foram 
			mantidas, porém os significados são diferentes, abaixo o que indica cada uma.

			source = número do câmera/nome do federado
			address = variável que controla se a câmera ou o "servidor" deve escutar o read()
			size = Tempo do vídeo
			data1 = Frame do vídeo
			data2 = Tempo do HLA (o que é feito pelo advanced time, por exemplo)
			data3 = Quantidade de faces encontradas
			data4 = flag que diz que faces foram encontradas (TALVEZ MUDAR ISSO PRA O QUANTIADE DE FACES)
			data5 = Tempo rodado na máquia (clock)
			data6 = livre... talvez seja o sendCrop (revisar)
			data7 = livre...
			data8 = livre...
			dataFrame = foto.

		*/
		string source;
		string address;
		string size;
		string data1;
		string data2;
		string data3;
		string data4;
		string data5;
		string data6;
		string data7;
		string data8;
		unsigned char dataFrame[NUM_ELEMENTS];
		//Vaŕiáveis compartilhadas do HLA
		
		void initializeHandles();
		void waitForUser();
		void enableTimePolicy();
		void publishAndSubscribe();
		RTI::ObjectHandle registerObject();
		void updateAttributeValues(string src, string addr, string size, string data1, string data2, string data3, string data4, string data5, string data6, string data7, string data8, unsigned char* dataFrame);
		void sendInteraction();
		
		void deleteObject( RTI::ObjectHandle objectHandle );
		double getLbts();
		double getCurrentTime();

		//void consultTerminalCam(unsigned c);
		//void stopConsultTerminalCam();
		//void screen();
		//void* readInfoCam()
};

#endif /*ARMVirtualBusFederate_H_*/
