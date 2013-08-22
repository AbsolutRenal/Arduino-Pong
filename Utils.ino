int getSign(float n){
  int ret = n / abs(n);
  if(ret == 0)
    ret = 1;
  return ret;
}
