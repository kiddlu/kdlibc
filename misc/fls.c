int fls(int word)
{
  #define CHAR_BIT 8

  if (word == 0)
    return 0;
  else 
    return (sizeof(word) * CHAR_BIT - clz(word));
}