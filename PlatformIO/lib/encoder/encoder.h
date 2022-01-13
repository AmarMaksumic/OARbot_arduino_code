class encoder {
public:
  encoder(unsigned int pin_) : SLAVE_SEL_ENC(pin_) { initEncoder();
                                                     clearEncoderCount(); };

  void initEncoder();
  long readEncoder();
  void clearEncoderCount();
private:
  unsigned int SLAVE_SEL_ENC;
};