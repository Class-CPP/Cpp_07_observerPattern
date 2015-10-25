#include <iostream>
#include <list>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
using ::std::list;
 
int kbhit(void){
	struct termios oldt, newt;
  	int ch;
  	int oldf;
 
  	tcgetattr(STDIN_FILENO, &oldt);
  		newt = oldt;
  		newt.c_lflag &= ~(ICANON | ECHO);
  	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  		oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  		ch = getchar();  
 
  	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  	if(ch != EOF){
    	ungetc(ch, stdin);
    		return 1;
  }
 
  return 0;
};
 

class button{
	public: 
		char value;
		bool state;
};
class EventHandler{
//private: 
public:
	virtual void onEvent(char eventData) = 0;

};

class MP3 : public EventHandler{
private: 
	int volumen;
	char key_plus;
	char key_minus;
	int serial_number;
public: 
	MP3(int serial_number, char key_plus,char key_minus){
		this->volumen = 50;
		this->serial_number =serial_number;
		this->key_plus = key_plus;
		this->key_minus = key_minus;
	}
	void increaseVolumen(void){
		volumen++;
	}
	void decreaseVolumen(void){
		volumen--;
	}
	int getVolumen(void){
		return volumen;
	}
	virtual void onEvent(char eventData){
		if(eventData == key_plus){
			this->increaseVolumen();
			std::cout<< "\n#serial:"<<serial_number <<"\n new voulmen"<< volumen << eventData<< std::endl;
		}else if(eventData == key_minus){
			this->decreaseVolumen();
			std::cout<< "\n#serial:"<<serial_number <<"\n new voulmen"<< volumen << eventData<< std::endl;
		
		}
	}

};
class Observable {
protected: 
	list<EventHandler*> observers; 
	void notifyAll(char eventData){
		for(auto observer : observers){
			observer->onEvent(eventData);
		}
	}
public: 
	void suscribe(EventHandler& eventHandler){
		observers.push_back(&eventHandler);
	}
	void unsuscribe(EventHandler& eventData);
	
};
class KeyBoardreader : public Observable{
public:
	void readloop(void){
		int counter=0;
		char c ='0';
		std::cout<< "prees any key to quit" << std::endl;
		while(c !='q'){
			while(kbhit() == 0){
				counter++;
			} 
			c = getchar();
			this->notifyAll(c);
		}
	}
};

int main(){
	MP3 mp3_1(1,'+','-');
	MP3 mp3_2(2,'p','m');	
	KeyBoardreader reader;
	reader.suscribe(mp3_1);
	reader.suscribe(mp3_2);
	reader.readloop();
		return 0;
}