int clz(int word)
{
  int i;

  if(!word)
    return 32;
  
  for(i = 0; word != 0; i++){
    word = word >> 1;
  }

  return 32 - i;
}