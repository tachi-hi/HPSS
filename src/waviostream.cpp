#include"waviostream.hpp"

namespace wav{
	template<> double realization(unsigned char x){return static_cast<double>(x + 128) / 256.0;};
	template<> double realization(signed short x) {return static_cast<double>(x)/32767.0;};
	template<> unsigned char quantize<unsigned char>(double x){return static_cast<unsigned char>(x * 256 - 128);};
	template<> signed short  quantize<signed short> (double x){return static_cast<signed short> (x * 32767);};
}

wavistream::wavistream(const char *fn, int buffer_size){
  filename = fn;
  fp = fopen(filename.c_str(), "r");
  if( !fp ){
    std::cerr << "cannot open " << filename << "!" <<std::endl;
    exit(1);
  }
  !fread(&header, sizeof(header), 1, fp);
  current_seek = 0;//sizeof(header);
  signal.init(header.n_channel, buffer_size);
}

wavistream::~wavistream(){
  fclose(fp);
}

wavostream::wavostream(const char *fn, int ch, int buffer_size){
  filename = fn;
  fp = fopen(filename.c_str(), "w");
  if( !fp ){
    std::cerr << "cannot open " << filename << "!" <<std::endl;
    exit(1);
  }
  signal_length = 0;
  char tmp[sizeof(wav::Header)];
  fwrite(tmp, 1, sizeof(wav::Header), fp);
  signal.init(ch, buffer_size);
  header.n_channel = ch;
  this->buffer_size = buffer_size;
}
wavostream::~wavostream(){
  write_header();
  fclose(fp);
}

void wavostream::set(double *x, int ch, int n){
//  double max = *std::max_element(&(x[0]), &(x[n]));
  for(int t = 0; t < n; t++){
    if(x[t] > 1.0){
      signal[ch].insert(1.0);
    }else if(x[t] < -1.0){
      signal[ch].insert(-1.0);
    }else{
      signal[ch].insert(x[t]);
    }
  }
}


void wavostream::write_header(void){
  if(header.sampling_rate == 0 || header.n_channel == 0 || header.bits_per_sample == 0){
    std::cerr << "sampling_rate, n_channel & bit_rate are not set!" << std::endl;
    exit(1);
  }

  header.riff[0] = 'R';

  header.riff[1] = 'I';
  header.riff[2] = 'F';
  header.riff[3] = 'F';
  header.filesize = (header.bits_per_sample / 8) * signal_length * header.n_channel + 36;
  header.wavefmt[0] = 'W';
  header.wavefmt[1] = 'A';
  header.wavefmt[2] = 'V';
  header.wavefmt[3] = 'E';
  header.wavefmt[4] = 'f';
  header.wavefmt[5] = 'm';
  header.wavefmt[6] = 't';
  header.wavefmt[7] = ' ';
  header.waveformat = header.bits_per_sample;
  header.pcm = 1;
  header.n_channel = header.n_channel;
  header.sampling_rate = header.sampling_rate;
  header.bytes_per_second = (header.bits_per_sample / 8) * header.n_channel * header.sampling_rate;
  header.block_per_sample = (header.bits_per_sample / 8) * header.n_channel;
  header.bits_per_sample = header.bits_per_sample;
  header.data[0] = 'd';
  header.data[1] = 'a';
  header.data[2] = 't';
  header.data[3] = 'a';
  header.n_byte = (header.bits_per_sample / 8) * signal_length * header.n_channel; 
  fseek(fp, 0, SEEK_SET);
  fwrite(&header, sizeof(header), 1, fp);

}

