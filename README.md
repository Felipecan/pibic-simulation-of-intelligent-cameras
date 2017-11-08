PIBIC
============

Simulação de câmeras inteligentes utilizando C++ com bibliotecas do OpenCV e High Level Architecture.  
--------------------------------------------------------------------------------------

Melhor maneira de explicar a implementação é ilustando o seguinte cenário:  
  

Em um ambiente monitorado por câmeras, principalmente pela segurança, em algum momento tornou-se necessário identificar certas faces no ambiente. Assim, o projeto visa simular câmeras onde é feito um processamento de detecção de faces e essas informações são compartilhadas entre as câmeras ou uma central através da rede. Essas câmeras podem ser simuladas em qualquer disposivo unindo plataformas diferentes.   
  
**Requisitos:** 
> - OpenCV para o processamento das imagens, preferencialmente a versão 3.1.0.  
> - High Level Architecture para comunicação entre os dispositivos. O padrão de comunicação utilizado pela departamento de defesa do EUA para intregar seus simuladores, o mesmo provê algumas ferramentas para o projeto.  

**Como executar:**
> - Primeiro executo o arquivo da pasta *virtual_bus_sender* (o mesmo simula uma central que envia informações para as câmeras procurarem): ./run_virtual_bus_sender 0  
> - Em seguida execute o arquivo da pasta *virtual_bus_receiver* (cada execução simulará uma câmera): ./run_virtual_bus_receiver número_da_câmera fonte_de_vídeo  
> - **Obs: A última câmera deve ter o número 2, para assim começar a simulação**  

