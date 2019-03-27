
//Vecinos Toroide
void devolverVecinostoroide(int l, int id, int *vecinos){


}


void vecinosToroide(int id, int *vecinos, int l){
	int f;
	int c;
	int vecino_n;
	int vecino_s;
	int vecino_d;
	int vecino_i;
	f=id/l;
	c=id%l;


	if(c==0){
		vecino_i = (f*l)+(l-1);
	}else{
		vecino_i = (f*l)+(c-1);
	}

	if(c==l-1){
		vecino_d = (f*l);
	}else{
		vecino_d = (f*l)+(c+1);
	}
	
	if(f==0){
		vecino_s = ((l-1)*l)+c;
	}else{
		vecino_s = ((f-1)*l)+c;
	}

	if(f==l-1){
		vecino_n = c;
	}else{
		vecino_n = ((f+1)*l)+c;
	}

}
