
#include "des.h"

void memcopy(char *dest, const char *src, int n){
  int k;
  for (k=0;k<n;k++)
    dest[k]=src[k];
}

char bit_get(const char *bits, int pos){
  return (bits[pos/8]>>(7-(pos%8)))&1;
}

void bit_set(char *bits, int pos, char etat){
  char masque=1<<(7-pos%8);
  if (etat)
    bits[pos/8]=bits[pos/8]|masque;
  else
    bits[pos/8]=bits[pos/8]&~masque;
}

void bit_xor(const char *bits1, const char *bits2, char *bitsx, int taille){
  int k;
  for (k=0;k<taille/8;k++)
    *(bitsx+k)=*(bits1+k)^*(bits2+k);
  if (taille%8!=0)
    *(bitsx+k)=(*(bits1+k)^*(bits2+k))&(256-(1<<(9-taille%8)));
}

void bit_rot_left(char *bits, char taille, char n){
  /*Fonctionne dans le cas où 0<=n<=8, taille%8!=0 et les bits non utilisés dans le dernier octet sont nuls. Ces trois points sont validés dans notre programme.*/
  char k, tmp=*bits>>(8-n);
  for (k=0;k<taille/8;k++)
    *(bits+k)=(*(bits+k)<<n)+(*(bits+k+1)>>(8-n));
  *(bits+k)=(*(bits+k)<<n)+(tmp<<(8-taille%8));
}

void permute(char *bits, char *corresp, int n){
  char temp[8];
  int i;
  for (i=0;i<n;i++)
    bit_set(temp, i, bit_get(bits, corresp[i]-1));
  memcopy(bits, temp, n/8);
}

void des_main(const char *source, char *cible, const char *cle, DesEorD sens){
  char sous_cles[16][7];
  char temp[8], cle_g[4], cle_d[4], bloc_g[6], bloc_d[6], bloc_f[6], bloc_x[6], bloc_s;
  int lig, col, i, j, k, p;
  if (source==NULL || cible==NULL || cle==NULL)
    return;
  memcopy(temp, cle, 8);
  permute(temp, Des_Transform, 56);
  for (k=0;k<4;k++){
    cle_g[k]=0;
    cle_d[k]=0;
  }
  for (j = 0; j < 28; j++)
    bit_set(cle_g, j, bit_get(temp, j));
  for (j = 0; j < 28; j++)
    bit_set(cle_d, j, bit_get(temp, j + 28));
  for (i = 0; i < 16; i++) {
    bit_rot_left(cle_g, 28, Des_Rotations[i]);
    bit_rot_left(cle_d, 28, Des_Rotations[i]);
    for (j = 0; j < 28; j++){
      bit_set(sous_cles[i], j, bit_get(cle_g, j));
      bit_set(sous_cles[i], j+28, bit_get(cle_d, j));
    }
    permute(sous_cles[i], Des_Permute, 48);
  }
  memcopy(temp, source, 8);
  permute(temp, Des_Initial, 64);
  memcopy(bloc_g, &temp[0], 4);
  memcopy(bloc_d, &temp[4], 4);
  for (i = 0; i < 16; i++) {
    memcopy(bloc_f, bloc_d, 4);
    permute(bloc_f, Des_Expansion, 48);
    if (sens == chiffrement)
      bit_xor(bloc_f, sous_cles[i], bloc_x, 48);
    else
      bit_xor(bloc_f, sous_cles[15-i], bloc_x, 48);
    memcopy(bloc_f, bloc_x, 6);
    p = 0;
    for (j = 0; j < 8; j++) {
      lig = (bit_get(bloc_f, (j * 6)+0) * 2) + 
        (bit_get(bloc_f, (j * 6)+5) * 1);
      col = (bit_get(bloc_f, (j * 6)+1) * 8) + 
        (bit_get(bloc_f, (j * 6)+2) * 4) +
        (bit_get(bloc_f, (j * 6)+3) * 2) + 
        (bit_get(bloc_f, (j * 6)+4) * 1);
      bloc_s = Des_Sbox[j][lig][col];
      for (k = 4; k < 8; k++) {
        bit_set(bloc_f, p, bit_get(&bloc_s, k));
        p++;
      }
    }
    permute(bloc_f, Des_Pbox, 32);
    bit_xor(bloc_f, bloc_g, bloc_x, 32);
    memcopy(bloc_g, bloc_d, 4);
    memcopy(bloc_d, bloc_x, 4);
  }
  memcopy(&cible[0], bloc_d, 4);
  memcopy(&cible[4], bloc_g, 4);
  permute(cible, Des_Final, 64);
}

void des_encipher(const char *clair, char *chiffre, const char *cle){
  des_main(clair, chiffre, cle, chiffrement);
}

void des_decipher(const char *chiffre, char *clair, const char *cle){
  des_main(chiffre, clair, cle, dechiffrement);
}

void des (char* vect_init, char* key1, char* key2, char* texte, char* chiffre, int size, int sens, int type, int mode)
{
  int i, cmp=0;
  char tampon_in[DES_BLOCK_SIZE], tampon_out[DES_BLOCK_SIZE];/*
  tampon_in=(char *)malloc(DES_BLOCK_SIZE*sizeof(char));
  tampon_out=(char *)malloc(DES_BLOCK_SIZE*sizeof(char));*/
  while (cmp != size){
    memcopy(tampon_in, texte+cmp*DES_BLOCK_SIZE,DES_BLOCK_SIZE);
    if (sens == CHIFFREMENT){
      if (mode == CBC){
        if (cmp == 0){
          for (i=0; i < DES_BLOCK_SIZE; ++i)
            tampon_in[i]=tampon_in[i] ^ vect_init[i];
        }
        else {
          for (i=0; i < DES_BLOCK_SIZE; ++i)
            tampon_in[i]=tampon_in[i] ^ chiffre[((cmp-1)*DES_BLOCK_SIZE)+i];
        }
      }
      des_encipher(tampon_in, tampon_out, key1);
      if (type == TRIPLE_DES){
        des_decipher(tampon_out, tampon_in, key2);
        des_encipher(tampon_in, tampon_out, key1);
      }
    }
    else if (sens == DECHIFFREMENT){
      des_decipher ( tampon_in, tampon_out, key1);
      if (type ==TRIPLE_DES){
        des_encipher(tampon_out, tampon_in, key2);
        des_decipher(tampon_in, tampon_out, key1);
      }
      if (mode == CBC){
        if (cmp == 0){
          for (i=0; i < DES_BLOCK_SIZE; ++i)
            tampon_out[i]=tampon_out[i] ^ vect_init[i];
        }
        else {
          for (i=0; i < DES_BLOCK_SIZE; ++i)
            tampon_out[i]=tampon_out[i]^ texte[((cmp-1)*DES_BLOCK_SIZE)+i];
        }
      }
    }
    memcopy(chiffre + cmp * DES_BLOCK_SIZE, tampon_out, DES_BLOCK_SIZE);
    cmp++;
  }/*
  free(tampon_in);
  free(tampon_out);*/
}

void des_crypt(char * key, char * texte, char* chiffre, int size)
{
  des (NULL, key,NULL,texte,chiffre,size, CHIFFREMENT, DES, ECB);
}

void des_decrypt(char * key, char * texte, char* chiffre, int size)
{
  des (NULL, key,NULL,texte,chiffre,size,DECHIFFREMENT, DES, ECB);
}

void des_crypt_cbc(char* vect_init, char* key, char* texte, char* chiffre, int size)
{
  des (vect_init, key,NULL,texte,chiffre,size, CHIFFREMENT, DES,CBC);

}

void des_decrypt_cbc(char* vect_init, char* key, char* chiffre, char* clair, int size)
{
  des (vect_init, key, NULL, chiffre, clair, size, DECHIFFREMENT, DES,CBC);
}

void tripledes_crypt(char * key1, char * key2, char * texte, char* chiffre,int size)
{
  des (NULL, key1,key2,texte,chiffre,size, CHIFFREMENT, TRIPLE_DES, ECB);
}

void tripledes_decrypt(char* key1, char* key2, char* texte, char* chiffre, int size)
{
  des (NULL, key1,key2,texte,chiffre,size, DECHIFFREMENT, TRIPLE_DES,ECB);
}

void tripledes_crypt_cbc(char * vect_init, char* key1, char* key2, char* texte, char* chiffre, int size)
{
  des (vect_init, key1,key2,texte,chiffre,size, CHIFFREMENT, TRIPLE_DES, CBC);
}

void tripledes_decrypt_cbc(char * vect_init, char* key1, char* key2, char* chiffre, char* clair, int size)
{
  des (vect_init, key1, key2,chiffre,clair,size, DECHIFFREMENT, TRIPLE_DES, CBC);
}


  char texte[8]="azerty";
  char chiffre[8];
  char dechiffre[8];
  char clef[8]="chabada";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Texte : ");
  Serial.println(texte);
  des_crypt(clef, texte, chiffre, 1);
  des_decrypt(clef, chiffre, dechiffre, 1);
  chiffre[7]=0;
  Serial.print("Chiffre : ");
  Serial.println(chiffre);
  Serial.print("Dechiffre : ");
  Serial.println(dechiffre);
  delay(1000);
}
