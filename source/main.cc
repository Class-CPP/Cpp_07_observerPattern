#include <iostream>
#include <list>
#include <math.h>

class File{
	public:
		virtual float read() = 0;
};

class SineFile : public File{
	private:
		int n;
		float freqHz;
		float freqSample;
	public:
		#define PI 3.1416
		SineFile(float freqHz, float freqSample){
			this->n = 0;
			this->freqHz = freqHz;
			this->freqSample= freqSample;
		}
		virtual float read(){
			float t = n/freqSample;
			float signal =  sin(t*2*PI*freqHz);
			this->n++;
			return signal;
		}
};

class Filter{
	public:
		virtual float process(float signal) = 0;
};

class NoneFilter : public Filter{
	public:
		virtual float process(float signal){
			return signal;
		}
};

class OutputDevice{
	public:
		virtual void write(float signal) = 0;
};

class PrinterOutputDevice : public OutputDevice{
	public:
		virtual void write(float signal){
			std::cout << "signal = " << signal << std::endl;
		}
};


class MP3{
	private:
		std::list<File*>::iterator songs;
		Filter* filter;
		OutputDevice* outputDevice;
	public:
		MP3(std::list<File*>::iterator& songs, Filter& filter, OutputDevice& outputDevice){
			this->songs = songs;
			this->filter = &filter;
			this->outputDevice = &outputDevice;
		}
		void play(void){
			auto file  = *songs;
			float songData = file->read();
			float filteredData = filter->process(songData);
			outputDevice->write(filteredData);
		}
		void next(void){
			songs++;
		}
		void back(void){
			songs--;
		}
};

int main(){
	SineFile file1(1,10);
	SineFile file2(2,10);
	std::list<File*> songs;
	songs.push_back(&file1);
	songs.push_back(&file2);

	NoneFilter filter;
	PrinterOutputDevice outputDevice;

	auto songsInterator = songs.begin();
	MP3 mp3(songsInterator, filter, outputDevice);


	for(int i = 0; i <= 10; i++){
		mp3.play();
	}
	std::cout << "new songs" <<  std::endl;
	mp3.next();
	for(int i = 0; i <= 10; i++){
		mp3.play();
	}
	std::cout << "new songs" <<  std::endl;
	mp3.back();
	for(int i = 0; i <= 10; i++){
		mp3.play();
	}

	return 0;
}
