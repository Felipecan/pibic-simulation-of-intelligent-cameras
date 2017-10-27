rm *.o run_virtual_bus_sender

#g++ -I/usr/local/include/hla13/ -O2 -g -Wall -DRTI_USES_STD_FSTREAM -c -fmessage-length=0 FederateAmbassador.cpp VirtualBusFederate.cpp main.cpp

#g++ -I/home/ppgi1/app/certi-CERTI-3_4_3/include/hla-1_3 -DRTI_USES_STD_FSTREAM -c FederateAmbassador.cpp VirtualBusFederate.cpp main.cpp



#g++ -I/home/felipe/Documentos/CERTI_HLA/CERTI-3.4.3-Source/include/hla-1_3 -I/home/felipe/Documentos/CERTI_HLA/CERTI-3.4.3-Source/build/include/hla-1_3 -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -DRTI_USES_STD_FSTREAM -o run_virtual_bus_sender FederateAmbassador.cpp VirtualBusFederate.cpp main.cpp -lRTI-NGd -lFedTimed -lHLAd -lCERTId -lopencv_imgcodecs -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lopencv_videoio

#g++ -I/home/felipe/Documentos/CERTI_HLA/CERTI-3.4.3-Source/include/hla-1_3 -I/home/felipe/Documentos/CERTI_HLA/CERTI-3.4.3-Source/build/include/hla-1_3 -L/usr/local/lib/ -g -DRTI_USES_STD_FSTREAM -o run_virtual_bus_sender FederateAmbassador.cpp VirtualBusFederate.cpp main.cpp -lRTI-NGd -lFedTimed -lHLAd -lCERTId `pkg-config opencv --cflags --libs`

g++ -std=c++11 -O3 -I/home/felipe/Documentos/sc/CERTI/include/hla-1_3 -I/home/felipe/Documentos/sc/CERTI/build/include/hla-1_3 -L/usr/local/lib/ -g -DRTI_USES_STD_FSTREAM -o run_virtual_bus_sender FederateAmbassador.cpp VirtualBusFederate.cpp main.cpp -lRTI-NGd -lFedTimed -lHLAd -lCERTId `pkg-config opencv --cflags --libs`


#g++ -L/home/ppgi1/app/openCV/opencv-2.4.9/build/lib/ -L/usr/local/lib -L/home/ppgi1/app/certi-CERTI-3_4_3/libRTI/hla-1_3 -o "run_virtual_bus_sender" VirtualBusFederate.o FederateAmbassador.o main.o  -lopencv_core -lRTI-NGd -lFedTimed -lHLAd -lCERTId


