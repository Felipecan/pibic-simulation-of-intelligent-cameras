#include <iostream>
#include "RTI.hh"
#include "fedtime.hh"

#include "FederateAmbassador.h"
#include "VirtualBusFederate.h"
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//#define LOG
//#define SV_IMG

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
	//advanceTime(fedamb->federateLookahead);
}


bool VirtualBusFederate::readData(string& source, string& addr, string& size, string &data1, string &data2, string &data3, string &data4, string &data5, string &data6, string &data7, string &data8, unsigned char* dataFrame)
{
	if(fedamb->hasReceivedData())
	{
		fedamb->getReceivedData(source, addr, size, data1, data2, data3, data5, data4, data6, data7, data8, dataFrame);
        //cout << ">>>> x=" << source << " y=" << addr;
        //cout << endl << ">>> Receiving data: " << data[0] << " " << data[1] << " " << data[2] << endl;
        //cout << "OI" << endl;
		return true;
	}
	else
		return false;
}

bool VirtualBusFederate::hasReceivedData(){
	return receivedData;
}


void VirtualBusFederate::loop (int iteracoes, char srcVideo[])
{

	//////////////////////////////////////////////////////////
	//////////////////CONFIGURAÇÃO DO LOG////////////////////
	ss.str(std::string());
	ss << federateName;
	
	#ifdef LOG
	string lg = "LOGS/"; 
	lg += ss.str();
	lg += "_Simulation_Log_";
	lg += __DATE__;
	lg += "_";
	lg += __TIME__;
	lg += ".csv";

	log.open(lg.c_str());
	log << "Number of faces,Video time,Video frame,HLA Simulation Time,Machine Time" << endl; 
  	#endif

	ss.str(std::string());
	/////////////////FIM DA CONFIGURAÇÃO///////////////////
	//////////////////////////////////////////////////////

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

  	if(!face_cascade.load(face_cascade_name))
	{ 
  		printf("--(!)Error loading\n"); 
  		exit(1);
  	}
 
  	if(((unsigned)srcVideo[0]) == '0')
  	{
  		VideoCapture v_aux(0);
  		v = v_aux;
  	}
  	else
  	{
  		VideoCapture v_aux(srcVideo);
  		v = v_aux;
  	}

	if(!v.isOpened())
	{
		cout << "Não encontrado\n";
		exit(1);
	}

	//v.set(CV_CAP_PROP_FPS, 10);
	//fps = v.get(CV_CAP_PROP_FPS);
	//cout << "Frames " << fps << endl;

	namedWindow("Video", CV_WINDOW_AUTOSIZE);

	start = clock();

	/*while(true)
	{

		if(v.set(CAP_PROP_POS_MSEC, 0)) // primeiro while e repetição de vídeo
		{
			cout << "time 0" << endl;
			loopM = true;
		}*/

 		while(loopM)
 		{	
			//std::cout << "federateName = " << atoi(this->federateName) << std::endl;
			//success = v.read(frame);

			if(!v.read(frame))
			{
				cout << "Não foi lido\n";
				cout << "Acabou" << endl;
				loopM = false;
				break;
			}

  			if(!draw)
  			{
  				imshow("Video", frame);

				if(waitKey(30) == 27)
				{
					cout << "Video desligado, saindo...\n";
					//cout << "Frame final " << v.get(CAP_PROP_POS_FRAMES);
					//data3 = 1;
					//writeData(s, a, size, data1, data2, data3, data4);
					//advanceTime(1.0);
					break;
				}	
	  		}
  			else
  			{
  				if(frameCount > FRAME_RATE)
  				{
					croppedImages.clear();
					frameCount = 0;

					cvtColor(frame, frame_gray, CV_BGR2GRAY);
  					equalizeHist(frame_gray, frame_gray);
  					face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
  					
  					capture = clock();

  					for( size_t i = 0; i < faces.size(); i++ )
	  				{
	  					//Size deltaSize( faces[i].width * 0.1f, faces[i].height * 0.1f );
	  					//Rect aux = faces[i] + deltaSize;
	  					croppedImages.push_back(frame((faces[i])));
		    			rectangle(frame, faces[i], CV_RGB(0, 255,0), 2);
	  				}	

  					imshow("Video", frame);

  					if(waitKey(30) == 27)
					{
						cout << "Video desligado, saindo...\n";
						break;
					}
  				}
  				else
  				{
  					frameCount++;

  					for( size_t i = 0; i < faces.size(); i++ )
	  				{
		    			rectangle(frame, faces[i], CV_RGB(0, 255,0), 2);
	    				//croppedImages.push_back(frame(faces[i]));
	  				}
  					
	  				imshow("Video", frame);

					if(waitKey(30) == 27)
					{
						cout << "Video desligado, saindo...\n";
						break;
					}
				}
  			}

			if(readData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame))
			{
				
				draw = false;
			
				if( ( strcmp( source.c_str(), federateName ) == 0 ) && ( address == "0" ) )
				{
					draw = true;
					//importante <<<<<<<<<<<<<<<<<<<<<
					//sendInfo para true e começar a esperar um data para cancelar a procura
					if(faces.size() <= 0)
					{  	
						//data2 = v.get(CAP_PROP_POS_MSEC);
						cout << "--------------------------\n";
						cout << "Face nao encontrada\n";
						cout << "Camera " << source << endl;

						data4 = "0";
						address = "1"; //definida para não enviar mensagens para o receiver 
						//timeVS = 0;

						//cout << "data1 = " << data1 << endl;
						stringstream ss_test;
						ss_test.str(std::string());
						ss_test << source;
						data8 = ss_test.str();
						writeData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
						//advanceTime(1.0);
						//cout << "if data1 = " << (int)data1 << endl;
					}
					else
					{	
						ss.str(std::string());
						ss << v.get(CAP_PROP_POS_MSEC);
						size = ss.str();

						ss.str(std::string());
						ss << v.get(CAP_PROP_POS_FRAMES);
						data1 = ss.str();

						ss.str(std::string());
						ss << faces.size();
						data3 = ss.str();

						ss.str(std::string());
						ss << getCurrentTime();
						data2 = ss.str();

						difTime = 1000*(capture - start)/((double)(CLOCKS_PER_SEC));
						ss.str(std::string());
						ss << difTime;
						data5 = ss.str();

						//LOG WRITE
						#ifdef LOG
						log << data3 << "," << size << "," << data1 << "," << data2 << "," << difTime << endl;//",";
						#endif

						cout << "--------------------------\n";
						cout << "Face encontrada\n";
						cout << "Camera " << source << endl;
						cout << "Time " << size << endl;
						cout << "M " << difTime << " ms"<< endl;

						data4 = "1";
						address = "1"; //definida para não enviar mensagens para o receiver 
						sendImg = true;

						int j = 0;

						for(int i = 0; i < faces.size(); i++)
						{
							int resolution = ((croppedImages[i].rows) * (croppedImages[i].cols));

							cout << "RESOLUTION " << resolution << endl;
							cout << "FACE " << i+1 << endl;

							namedWindow("Receive Crop", WINDOW_AUTOSIZE);
							imshow("Receive Crop", croppedImages[i]);
							if(waitKey(1) == 27)
							{
								cout << "Saindo\n";
								break;
							}

							for(unsigned x = (croppedImages[i].cols-1); (int)x >= 0; x--)
							{
								for(unsigned y = (croppedImages[i].rows-1); (int)y >= 0; y--)
								{
									Vec3b s = croppedImages[i].at<Vec3b>(Point(x, y));
									dataFrame[j+0] = x;
									dataFrame[j+1] = y;
									//cout << "X " << dataFrame[j+0] << " Y " << dataFrame[j+1] << endl;
									dataFrame[j+2] = s.val[0];
									dataFrame[j+3] = s.val[1];
									dataFrame[j+4] = s.val[2];
									j += 5;
								}
							}
							//destroyWindow("Receive Crop");
							
							string strNameImage = "CROPS/";
							ss.str(std::string());
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
							imwrite(strNameImage, croppedImages[i]);
							#endif
							
							cout << "FACE " << i+1 << endl;
							cout << "enviando... ";
							//countBits = (((croppedImages[i].rows) * (croppedImages[i].cols))*3*2);
							//log << sizeof(dataFrame) << endl;
							stringstream ss_test;
							ss_test.str(std::string());
							ss_test << source;
							data8 = ss_test.str();
							writeData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
							cout << "enviado\n";
							j = 0;
						}
						croppedImages.clear();
					}
				}
				//croppedImages.clear();
				ss.str(std::string()); // limpa a string
			}
			//croppedImages.clear();
			advanceTime(1.0);
		}
	//}
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

	////////////////////////////////
	// 4. announce the sync point //
	////////////////////////////////
	// announce a sync point to get everyone on the same page. if the point
	// has already been registered, we'll get a callback saying it failed,
	// but we don't care about that, as long as someone registered it

	if( strcmp( federateName, "2" ) == 0) //// 9999 é a última câmera, então coloca READY_TO_RUN
	{

		//rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, "hi!" );

	}
	else//Não sendo a última câmera, coloca o nome normal
	{

		//rtiamb->registerFederationSynchronizationPoint( federateName, "hi!" );

	}
	while( fedamb->isAnnounced == false )
	{
		rtiamb->tick();
	}

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
	enableTimePolicy();
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


	
	//cout << "**** Atributos adicionados ****" << endl;
	
	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
	RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	rtiamb->updateAttributeValues(oHandle, *attributes, time, federateName );
	


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

/*void VirtualBusFederate::writeFrame(unsigned)
{

	data[0] = framesRect[0].cols-1;
	data[1] = framesRect[0].rows-1;

	for (int x = image.cols-1; x >= 0; x--)
	{
	
		for (int y = image.rows-1; y >= 0; y--)
		{

			data[]

		}

	}

}*/

/*void VirtualBusFederate::writeFrame(unsigned (data&)[38])
{



}*/

