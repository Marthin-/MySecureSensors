struct RSAKey{
  unsigned long e;
  unsigned long n;
  unsigned long d;
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
  Serial.println("Compute RSA keys using prime numbers");
  unsigned int p,q; //(unsigned int : from 0 to 65,535)
  p=generatePrime();
  q=generatePrime();
  Serial.print("Using p=");
  Serial.print(p);
  Serial.print(" ; q=");
  Serial.println(q);
  Serial.println("computing...");
  //calcul clés
  unsigned long n= p*q;
  unsigned long phi=(p-1)*(q-1);
  unsigned long e;
  e=random(3,phi);//2 and phi are not coprime
  while(gcd(phi,e)!=1){
    if(e==phi){
      e=random(3,phi);
    }
    e++;
  }
  unsigned long d;
  d=random(3,phi);
  while(((e*d)%phi)!=1){
    if(d==phi){
      d=random(3,phi);
    }
    d++;
  }
  Serial.println("RSA keys generated :");
  Serial.print("Public key : (");
  Serial.print(e);
  Serial.print(",");
  Serial.print(n);
  Serial.print(") ; Private key : (");
  Serial.print(d);
  Serial.println(")");

  RSAKey MyKey= { e, n, d};  
}

void loop() {
  // put your main code where your mouth is

}

unsigned int generatePrime(){  
  unsigned int a;
  a=random(2837,65533);
  while(isPrime(a)==false){
    a=random(2837,65533);
  }
  return a;
}

boolean isPrime(unsigned int a){
  for(int i=3;i<a;i++){
    if(a%i==0){
      return false;
    }
  }
  return true;
}

unsigned long gcd(unsigned long a, unsigned long b){
  unsigned long num, denom,rem,gcd;
  if(a>b){
    
    num=a;
    denom=b;
  }
  else{
    num=b;
    denom=a;
  }
  rem=num%denom;
  while(rem!=0){
    num=denom;
    denom=rem;
    rem=num%denom;
  }
  return denom;
}

