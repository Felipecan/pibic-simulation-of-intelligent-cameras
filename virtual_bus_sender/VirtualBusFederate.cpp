#include <iostream>
#include "RTI.hh"
#include "fedtime.hh"

#include "FederateAmbassador.h"
#include "VirtualBusFederate.h"
#include <string>
#include <unistd.h>

//#define LOG
//#define SV_IMG

#include <sstream>

using namespace std;
using namespace cv;


VirtualBusFederate::VirtualBusFederate()
{
}

VirtualBusFederate::~VirtualBusFederate()
{
	if( this->fedamb )
		delete this->fedamb;
}


///////////////////////////////////////////////////////////////////////////
////////////////////////// Meus Metodos /////////////////////////
///////////////////////////////////////////////////////////////////////////

void VirtualBusFederate::writeData(string src, string addr, string size, string data1, string data2, string data3, string data4, string data5, string data6, string data7, string data8, unsigned char* dataFrame){
	updateAttributeValues(src, addr, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
//	cout << "2.1: " << endl;
    //cout << ">>>> s=" << src << " a=" << addr;
	//cout << endl << ">>> Sending data: " << data[0] << " " << data[1] << " " << data[2] << endl;
	//cout << "time " << fedamb->federateTime << endl;
	advanceTime(fedamb->federateLookahead);
}


bool VirtualBusFederate::readData(string& source, string& addr, string& size, string &data1, string &data2, string &data3, string &data4, string &data5, string &data6, string &data7, string &data8, unsigned char* dataFrame){
	if(fedamb->hasReceivedData())
	{
		fedamb->getReceivedData(source, addr, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
		return true;
	}
	else
		return false;
}

bool VirtualBusFederate::hasReceivedData(){
	return receivedData;
}


void VirtualBusFederate::loop (int iteracoes)
{	

	//////////////////////////////////////////////////////////
	//////////////////CONFIGURAÇÃO DO LOG////////////////////

	#ifdef LOG
	string lg = "LOGS/"; 
	lg += __DATE__;
	lg += "_";
	lg += __TIME__;
	lg += ".csv";

	log.open(lg.c_str());
	log << "Terminal,Number of faces,Video time,Video frame,HLA Simulation Time,Machine Time,Time of receipt" << endl;
	#endif

	/////////////////FIM DA CONFIGURAÇÃO///////////////////
	//////////////////////////////////////////////////////	

	Mat image;
	bool d = true;
	int i = 0;
	int j = 0;
	string number_cam_l = "1";
	unsigned int countCrop = 0;

	unsigned timeMachine = 0;

	
	
	start = clock();

	while(d)
	{	

		//advanceTime(1.0);
		cout << "\n>>>>>>>>>>>> Terminal Servidor <<<<<<<<<<<<\n" << endl;
		//cout << "Digite o numero da camera, em seguida [1] para consulta-la ou [0] para parar consulta" << endl;
		
		source = number_cam_l;

		cout << "Consultando camera numero " << source << endl;
		
		address = "0"; //habilita o envio do servidor

		//tratar entrada ainda....
		//cin >> opcao;

		//cout << "Consultando temintal " << numberCam << endl;
		sleep(2); //pequena pausa para processamento

		stringstream ss_test;ss_test.str(std::string());
		ss_test.str(std::string());
		ss_test << "0";
		size = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data1 = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data2 = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data3 = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data4 = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data5 = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data6 = ss_test.str();
		ss_test << "0";
		data7 = ss_test.str();
		ss_test.str(std::string());
		ss_test << "0";
		data8 = ss_test.str();

		writeData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
		//advanceTime(1.0);
		/*if(fedamb->hasReceivedData())
			cout << "CHEGOU DADOS AQUI" << endl;*/

		if(readData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame)) 
		{
			//cout << "Hello World" << endl;
			if(source == number_cam_l && (address != "0"))
			{
				if(data4 == "1")
				{		
					capture = clock();

					cout << "\n>>>>> FACE ENCONTRADA!! <<<<<\n";

					cout << "Terminal " << source << endl;
					cout << "Amount faces " << data3 << endl;
					cout << "Video time " << size << " ms" <<  endl;
					cout << "Video frame " << data1 << endl;
					cout << "HLA time " << data2 << endl;
					difTime = 1000*(capture - start)/((double)(CLOCKS_PER_SEC));
					cout << "Machine time " << difTime << endl;

					#ifdef LOG
					log << source << ",";	
					log << data3 << ",";
					log << size << ",";
					log << data1 << ",";
					log << data2 << ",";
					log << difTime << ',';
					log << difTime/1000 << endl;
					#endif

					cout << ">>>>> FACE ENCONTRADA!! <<<<<\n";
					
					//cout << "\n" << endl;

					cout << "Inicio do transferência das imagens...\n";
					
					int j = 0;
					//cout << "X " << dataFrame[j+0] << " Y " << dataFrame[j+1] << endl;
					
					for(int i = 0; i < stoi(data3, nullptr, 10); i++)
					{
						image = Mat::zeros(((unsigned int)dataFrame[j+0]+1), ((unsigned int)dataFrame[j+1]+1), CV_8UC3);
						cout << "Processando imagem...\n";
						cout << "FACE " << i+1 << endl;

						do
						{
							Vec3b s;
							s.val[0] = dataFrame[j+2];
							s.val[1] = dataFrame[j+3];
							s.val[2] = dataFrame[j+4];

							image.at<Vec3b>((unsigned int)dataFrame[j+1], (unsigned int)dataFrame[j+0]) = s;

							if( !( (dataFrame[j+0] == 0) && ( dataFrame[j+1] == 0) ) )
							{
								j += 5;
							//cout << "J " << j << endl;
							}
							else
							{
								break;	
							}
						}
						while(true);

						stringstream ss;
						ss << source;

						string IC = ss.str();

						IC += " Image Capture ";

						//ss.str(std::string());
						//ss << (i+1);
						//IC += ss.str();

						//IC += " ";

						ss.str(std::string());
						ss << size;
						IC += ss.str();

						cout << "Exibindo imagem\n";

						namedWindow("Image Capture", WINDOW_AUTOSIZE);
						imshow("Image Capture", image);

						j = 0;
					
						//waitKey(3000);
						if(waitKey(1) == 27)
						{
							cout << "Saindo\n";
							break;
						}
						//destroyWindow("Image Capture");
						

						ss.str(std::string());
						string strNameImage = "CROPS/";
						ss << source;
						strNameImage += ss.str();
						strNameImage += "_";
						ss.str(std::string());
						ss << data2;
						strNameImage += ss.str();
						strNameImage += "_";
						strNameImage += __DATE__;
						strNameImage += "_";
						strNameImage += __TIME__;
						strNameImage += "_";
						ss.str(std::string());
						ss << countCrop;
						strNameImage += ss.str();
						strNameImage += ".png";
						ss.str(std::string());
						countCrop++;

						#ifdef SV_IMG
						imwrite(strNameImage, image);
						#endif
						
						/*if( (i+1) < amountFaces)
							while(!readData(numberCam, server, timeVS, timeVF, timeFed, amountFaces, found, timeMachine, sendCrop, data7, data8, dataFrame));*/
						
						if(i < (stoi(data3, nullptr, 10)-1))
						{
							cout << "Recebendo a próxima... ";

							readData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);

							receive = clock();
							dif_time_frame_receive = (receive - start)/((double)CLOCKS_PER_SEC);	
							
							#ifdef LOG
							log << source << ",";
							log << data3 << ",";
							log << size << ",";
							log << data1 << ",";
							log << data2 << ",";
							log << difTime << ',';
							log << dif_time_frame_receive << endl;
							#endif
						}
						cout << "Recebida\n";
					}

					cout << "Fim da transferência das fotos\n";
					cout << ">>>>> FACE ENCONTRADA!! <<<<<\n\n\n";

					//advanceTime(1.0);
				}
				else
				{
					cout << "\n" << endl;
					cout << ">>>>> FACE NAO ENCONTRADA!! <<<<<\n";
					cout << "Terminal " << source << endl;
					//cout << "Video time " << timeVS << endl;
					cout << ">>>>> FACE NAO ENCONTRADA!! <<<<<\n";
					cout << "\n" << endl;
					//advanceTime(1.0);
					//cout << "\n\n";
					//cout << " -- data1 " << data1 << endl;
				}
			}

			/*if(data4 == 1)
			{	

				cout << "A camera informada nao consta no sistema" << endl;
				data4 = 0;

			}*/

			//advanceTime(1.0);
		}
    
       	//advanceTime(1.0);
		/*if(j == 3)
		{

			data4 = 1; 
			cout << "EVIANDO SOLICITACAO PARA CAMERA DETECTAR\n";

		}
		else
		{

			data4 = 0;
			cout << "ENVIANDO SOLICITACAO PARA PARAR A DETECCAO\n";
			j = -1;
				
		}*/


       	if(i == 15)
       		number_cam_l = "2";
       	else if(i == 28)
       		number_cam_l = "0";
       	else if(i == 45)
       	{
       		number_cam_l = "1";
       		i = -1;
       		cout << "Acabou" << endl;
       		break;
       	}
       	//cout << "i: " << i << endl;
       	i++;
       	j++;
	}
}

double VirtualBusFederate::getCurrentTime()
{
	
	return fedamb->federateTime;
	
}


///////////////////////////////////////////////////////////////////////////
////////////////////////// Main Simulation Method /////////////////////////
///////////////////////////////////////////////////////////////////////////
void VirtualBusFederate::runFederate( char* federateName )
{
	//this->federateName = 
	strcpy(this->federateName, federateName);

	/////////////////////////////////
	// 1. create the RTIambassador //
	/////////////////////////////////
	this->rtiamb = new RTI::RTIambassador();

	//////////////////////////////////////////
	// 2. create and join to the federation //
	//////////////////////////////////////////
	// create
	// NOTE: some other federate may have already created the federation,
	//       in that case, we'll just try and join it
	try
	{
		rtiamb->createFederationExecution( "ARMVirtualBusFederation", "virtualbus.fed" );
		cout << "Created Federation" << endl;
	}
	catch( RTI::FederationExecutionAlreadyExists exists )
	{
		cout << "Didn't create federation, it already existed" << endl;
	}

	////////////////////////////
	// 3. join the federation //
	////////////////////////////
	// create the federate ambassador and join the federation
	this->fedamb = new FederateAmbassador(federateName);
	rtiamb->joinFederationExecution( federateName, "ARMVirtualBusFederation", fedamb );
	cout << "Joined Federation as " << federateName << endl;

	// initialize the handles - have to wait until we are joined
	initializeHandles();

	cout << "initializeHandles "  << endl;


	////////////////////////////////
	// 4. announce the sync point //
	////////////////////////////////
	// announce a sync point to get everyone on the same page. if the point
	// has already been registered, we'll get a callback saying it failed,
	// but we don't care about that, as long as someone registered it
	//rtiamb->registerFederationSynchronizationPoint( federateName, "" );
	while( fedamb->isAnnounced == false )
	{
		rtiamb->tick();
	}
	//cout << "registerFederationSynchronizationPoint "  << endl;

	// WAIT FOR USER TO KICK US OFF
	// So that there is time to add other federates, we will wait until the
	// user hits enter before proceeding. That was, you have time to start
	// other federates.
	//waitForUser();

	///////////////////////////////////////////////////////
	// 5. achieve the point and wait for synchronization //
	///////////////////////////////////////////////////////
	// tell the RTI we are ready to move past the sync point and then wait
	// until the federation has synchronized on
	/*while( fedamb->isAnnounced == false )
	{
		rtiamb->tick();
	}*/
	rtiamb->synchronizationPointAchieved( READY_TO_RUN );
	cout << "Achieved sync point: " << READY_TO_RUN << ", waiting for federation..." << endl;
	while( fedamb->isReadyToRun == false )
	{
		rtiamb->tick();
	}

	/////////////////////////////
	// 6. enable time policies //
	/////////////////////////////
	// in this section we enable/disable all time policies
	// note that this step is optional!
	//enableTimePolicy();
	cout << "Time Policy Enabled" << endl;

	//////////////////////////////
	// 7. publish and subscribe //
	//////////////////////////////
	// in this section we tell the RTI of all the data we are going to
	// produce, and all the data we want to know about
	publishAndSubscribe();
	cout << "Published and Subscribed" << endl;

	/////////////////////////////////////
	// 8. register an object to update //
	/////////////////////////////////////
	oHandle = registerObject();
	cout << "Registered Object, handle=" << oHandle << endl;

	


	
}


void VirtualBusFederate::finalize(){

	log.close();

	//////////////////////////////////////
	// 10. delete the object we created //
	//////////////////////////////////////
	deleteObject(oHandle);
	cout << "Deleted Object, handle=" << oHandle << endl;

	////////////////////////////////////
	// 11. resign from the federation //
	////////////////////////////////////
	rtiamb->resignFederationExecution( RTI::NO_ACTION );
	cout << "Resigned from Federation" << endl;

	////////////////////////////////////////
	// 12. try and destroy the federation //
	////////////////////////////////////////
	// NOTE: we won't die if we can't do this because other federates
	//       remain. in that case we'll leave it for them to clean up
	try
	{
		rtiamb->destroyFederationExecution( "ARMVirtualBusFederation" );
		cout << "Destroyed Federation" << endl;
	}
	catch( RTI::FederationExecutionDoesNotExist dne )
	{
		cout << "No need to destroy federation, it doesn't exist" << endl;
	}
	catch( RTI::FederatesCurrentlyJoined fcj )
	{
		cout << "Didn't destroy federation, federates still joined" << endl;
	}

	//////////////////
	// 13. clean up //
	//////////////////
	delete this->rtiamb;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Helper Methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
 * This method will get all the relevant handle information from the RTIambassador
 */
void VirtualBusFederate::initializeHandles()
{
	this->aHandle  = rtiamb->getObjectClassHandle( "VirtualBus.port" );
	this->srcHandle = rtiamb->getAttributeHandle( "source", aHandle );
	this->addrHandle = rtiamb->getAttributeHandle( "address", aHandle );
	this->sizeHandle = rtiamb->getAttributeHandle( "size", aHandle );
	this->data1Handle = rtiamb->getAttributeHandle( "data1", aHandle );
	this->data2Handle = rtiamb->getAttributeHandle( "data2", aHandle );
	this->data3Handle = rtiamb->getAttributeHandle( "data3", aHandle );
	this->data4Handle = rtiamb->getAttributeHandle( "data4", aHandle );
	this->data5Handle = rtiamb->getAttributeHandle( "data5", aHandle );
	this->data6Handle = rtiamb->getAttributeHandle( "data6", aHandle );
	this->data7Handle = rtiamb->getAttributeHandle( "data7", aHandle );
	this->data8Handle = rtiamb->getAttributeHandle( "data8", aHandle );
	this->dataFrameHandle = rtiamb->getAttributeHandle( "dataFrame", aHandle );
	
}

/*
 * Blocks until the user hits enter
 */
void VirtualBusFederate::waitForUser()
{
	cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << endl;
	string line;
	getline( cin, line );
}

/*
 * This method will attempt to enable the various time related properties for
 * the federate
 */
void VirtualBusFederate::enableTimePolicy()
{
	////////////////////////////
	// enable time regulation //
	////////////////////////////
	RTIfedTime federateTime = fedamb->federateTime;
	RTIfedTime lookahead = fedamb->federateLookahead;
	rtiamb->enableTimeRegulation( federateTime, lookahead );

	// tick until we get the callback
	while( fedamb->isRegulating == false )
	{
		rtiamb->tick();
	}

	/////////////////////////////
	// enable time constrained //
	/////////////////////////////
	rtiamb->enableTimeConstrained();

	// tick until we get the callback
	while( fedamb->isConstrained == false )
	{
		rtiamb->tick();
	}
}

/*
 * This method will inform the RTI about the types of data that the federate will
 * be creating, and the types of data we are interested in hearing about as other
 * federates produce it.
 */
void VirtualBusFederate::publishAndSubscribe()
{
	////////////////////////////////////////////
	// publish all attributes of ObjectRoot.robot //
	////////////////////////////////////////////
	// before we can register instance of the object class ObjectRoot.robot and
	// update the values of the various attributes, we need to tell the RTI
	// that we intend to publish this information

	// package the information into a handle set
	RTI::AttributeHandleSet *attributes = RTI::AttributeHandleSetFactory::create( 12 );
	
	attributes->add( this->srcHandle );
	attributes->add( this->addrHandle );	
	attributes->add( this->sizeHandle );
	attributes->add( this->data1Handle );
	attributes->add( this->data2Handle );
	attributes->add( this->data3Handle );
	attributes->add( this->data4Handle );
	attributes->add( this->data5Handle );
	attributes->add( this->data6Handle );
	attributes->add( this->data7Handle );
	attributes->add( this->data8Handle );
	attributes->add( this->dataFrameHandle );
	
	
	// do the actual publication
	rtiamb->publishObjectClass( this->aHandle, *attributes );
	//cout << " >>>>>>>>>> Atributos publicados <<<<<<<<<<" << endl;
	


	/////////////////////////////////////////////////
	// subscribe to all attributes of ObjectRoot.Robot //
	/////////////////////////////////////////////////
	// we also want to hear about the same sort of information as it is
	// created and altered in other federates, so we need to subscribe to it
	rtiamb->subscribeObjectClassAttributes( this->aHandle, *attributes );
	//cout << " >>>>>>>>>> Atributos subinscritos <<<<<<<<<<" << endl;
	

	/////////////////////////////////////////////////////
	// publish the interaction class InteractionRoot.X //
	/////////////////////////////////////////////////////
	// we want to send interactions of type InteractionRoot.X, so we need
	// to tell the RTI that we're publishing it first. We don't need to
	// inform it of the parameters, only the class, making it much simpler

	// do the publication
	//rtiamb->publishInteractionClass( this->xHandle );

	////////////////////////////////////////////////////
	// subscribe to the InteractionRoot.X interaction //
	////////////////////////////////////////////////////
	// we also want to receive other interaction of the same type that are
	// sent out by other federates, so we have to subscribe to it first
	//rtiamb->subscribeInteractionClass( this->xHandle );

	// clean up
	delete attributes;
}

/*
 * This method will register an instance of the class ObjectRoot.A and will
 * return the federation-wide unique handle for that instance. Later in the
 * simulation, we will update the attribute values for this instance
 */
RTI::ObjectHandle VirtualBusFederate::registerObject()
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle("VirtualBus.port") );
}

/*
 * This method will update all the values of the given object instance. It will
 * set each of the values to be a string which is equal to the name of the
 * attribute plus the current time. eg "aa:10.0" if the time is 10.0.
 * <p/>
 * Note that we don't actually have to update all the attributes at once, we
 * could update them individually, in groups or not at all!
 */
void VirtualBusFederate::updateAttributeValues(string src, string addr, string size, string data1, string data2, string data3, 
											   string data4, string data5, string data6, string data7, string data8, unsigned char* dataFrame)
{
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in, as you can see
	// this is quite a lot of work
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create( 12 );

	// generate the new values
	// we use EncodingHelpers to make things nice friendly for both Java and C++

	//cout << "**** Adicionando atributos ****" << endl;

	attributes->add( srcHandle, src.c_str(), (RTI::ULong)(src.size()+1));
	attributes->add( addrHandle, addr.c_str(), (RTI::ULong)(addr.size()+1));
	attributes->add( sizeHandle, size.c_str(), (RTI::ULong)(size.size()+1));
	attributes->add( data1Handle, data1.c_str(), (RTI::ULong)(data1.size()+1));
	attributes->add( data2Handle, data2.c_str(), (RTI::ULong)(data2.size()+1));
	attributes->add( data3Handle, data3.c_str(), (RTI::ULong)(data3.size()+1));
	attributes->add( data4Handle, data4.c_str(), (RTI::ULong)(data4.size()+1));
	attributes->add( data5Handle, data5.c_str(), (RTI::ULong)(data5.size()+1));
	attributes->add( data6Handle, data6.c_str(), (RTI::ULong)(data6.size()+1));
	attributes->add( data7Handle, data7.c_str(), (RTI::ULong)(data7.size()+1));
	attributes->add( data8Handle, data8.c_str(), (RTI::ULong)(data8.size()+1));
	attributes->add( dataFrameHandle, (char*)dataFrame, (RTI::ULong)(sizeof(unsigned char)) * NUM_ELEMENTS);
	
	//cout << "3: " << endl;
	
	
	//cout << "**** Atributos atualizados ****" << endl;
	
	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
	RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	rtiamb->updateAttributeValues(oHandle, *attributes, time, federateName );
	
	//cout << "4: " << endl;

	// clean up
	delete attributes;
}





/*
 * This method will send out an interaction of the type InteractionRoot.X. Any
 * federates which are subscribed to it will receive a notification the next time
 * they tick(). Here we are passing only two of the three parameters we could be
 * passing, but we don't actually have to pass any at all!
 */
void VirtualBusFederate::sendInteraction()
{
	/*	
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in
	RTI::ParameterHandleValuePairSet *parameters = RTI::ParameterSetFactory::create( 2 );

	// generate the new values
	char xaValue[16], xbValue[16];
	sprintf( xaValue, "xa:%f", getLbts() );
	sprintf( xbValue, "xb:%f", getLbts() );
	parameters->add( xaHandle, xaValue, (RTI::ULong)strlen(xaValue)+1 );
	parameters->add( xbHandle, xbValue, (RTI::ULong)strlen(xbValue)+1 );

	//////////////////////////
	// send the interaction //
	//////////////////////////
	rtiamb->sendInteraction( xHandle, *parameters, "hi!" );

	// if you want to associate a particular timestamp with the
	// interaction, you will have to supply it to the RTI. Here
	// we send another interaction, this time with a timestamp:
	RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	rtiamb->sendInteraction( xHandle, *parameters, time, "hi!" );

	// clean up
	delete parameters;
	*/
}

/*
 * This method will request a time advance to the current time, plus the given
 * timestep. It will then wait until a notification of the time advance grant
 * has been received.
 */
void VirtualBusFederate::advanceTime( double timestep )
{
	
	// request the advance
	fedamb->isAdvancing = true;
	RTIfedTime newTime = (fedamb->federateTime + timestep);
	rtiamb->timeAdvanceRequest( newTime );

	// wait for the time advance to be granted. ticking will tell the
	// LRC to start delivering callbacks to the federate
	while( fedamb->isAdvancing )
	{
		rtiamb->tick();
	}
}




/*
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void VirtualBusFederate::deleteObject( RTI::ObjectHandle objectHandle )
{
	rtiamb->deleteObjectInstance( objectHandle, NULL );
}

double VirtualBusFederate::getLbts()
{
	return (fedamb->federateTime + fedamb->federateLookahead);
}

/*void VirtualBusFederate::consultTerminalCam(unsigned* numberCam, unsigned* server, unsigned* size, unsigned* data1, unsigned* data2, unsigned* data3, unsigned* data4)
{

	*numberCam = 0;
	*server = 0;
	*data2 = 1;

	*size = 1;
	*data1 == 3;
	*data3 == 3;
	*data4 == 3;

	bool loop == true;

	cout << "Digite o numero da camera para consulta-la " << endl;
	cin >> *numberCam;

	writeData(*numberCam, *server, *size, *data1, *data2, *data3, *data4);

	cout < "ATENCAO digitar [2] para parar a consulta" << endl;

	while(loop)
	{

		if((readData(numberCam, server, size, data, data1, data2, data3, data4)) && (*server != 0))
		{

			if(*data1 == 1)
			{

				cout << "\n" << endl;
				cout < "ATENCAO digitar [2] para parar a consulta" << endl;
				cout << ">>>>> FACE ENCONTRADA!! <<<<<\n";
				cout << "Terminal " << *numberCam << endl;
				//cout << "Time " << getCurrentTime() << endl;
				cout << ">>>>> FACE ENCONTRADA!! <<<<<\n";
				cout << "\n" << endl;

			}
			else
			{

				cout << "\n" << endl;
				cout < "ATENCAO digitar [2] para parar a consulta" << endl;
				cout << ">>>>> FACE NAO ENCONTRADA!! <<<<<\n";
				cout << "Terminal " << numberCam << endl;
				//cout << "Time " << getCurrentTime() << endl;
				cout << ">>>>> FACE NAO ENCONTRADA!! <<<<<\n";
				cout << "\n" << endl;

			}

		}

	}

}

void VirtualBusFederate::stopConsultTerminalCam()
{



}

void VirtualBusFederate::screen()
{



}*/
