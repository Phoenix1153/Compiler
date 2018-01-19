const char _o='o',_r='r',_z='z',_l='l',_i='i',_b='b';
const int xixi=19,haha=26,hugao=8,maogao=17;
const int history_process=+1;
const int minus1=-1;

int maxval[233],minval[233],sum[233];
int data[233];
int n;
int qsort_data[233],m;

int partition(int low, int high){
    int key,flg,i;
    key = qsort_data[low];
	i = 0;
	do{
		flg=1;
    	if(low>=high) flg=0;
    	if(qsort_data[high]<key) flg=0;
        if(flg)  {
			i = 0;
			do{
				high=high-1;
				if(low>=high) flg=0;
				if(qsort_data[high]<key) flg=0;
				i = i + 1;
			}while(flg)
        
    	}
        if(low<high) {
            qsort_data[low] = qsort_data[high];
            low=low+1;
        }
        
        flg=0;
        if(low<high) if(qsort_data[low]<=key) flg=1;
		if(flg) {
			i = 0;
			do{
				low=low+1;
            flg=0;
            if(low<high) if(qsort_data[low]<=key) flg=1;
				i = i + 1;
			}while(flg)
        }
        if(low<high) {
            qsort_data[high] = qsort_data[low];
            high=high-1;
        }
		i = i +1;
	}while(low<high)
    qsort_data[low] = key;
    return(low);
}

int gcd(int a,int b) {
	if(b==0) return(a);
	return(gcd(b,a-a/b*b));
}




void qsort(int start, int end){
    int pos;
    if (start<end){
        pos = partition(start, end);
        qsort(start,pos-1);
        qsort(pos+1,end);
    }
    return;
}

void calculate_e() {
	const int maxn=233,max_iterate=233;
	int tmp[233],ans[233];
	int i,j,k,rem;
	i = 0;
	do{
		tmp[i]=0; ans[i]=0;
		i = i + 1;
	}while(i<maxn)
	
	ans[0]=2; tmp[0]=1;
	i = 1;
	do{
		rem=0;
		j = 0;
		do{
			rem=rem*10+tmp[j];
			tmp[j]=rem/i;
			rem=rem-rem/i*i;
			j = j +1;
		}while(j<maxn)

		j=maxn-1;
		do{
			ans[j]=ans[j]+tmp[j];
			if(ans[j]>10) {
				ans[j]=ans[j]-10;
				ans[j-1]=ans[j-1]+1;
			}
			j = j - 1;
		}while(j>=1)
		i = i + 1;
	}while(i<=max_iterate)	
	printf(ans[0]);
	printf(".");
	i = 0;
	do{
		printf(ans[i]);
		i = i + 1;
	}while(i<10)
} 

void weird_expressions_test() {
	printf(--2++3);
	printf(-2+-5);
	printf(0);
	printf(+-1);
	printf(-+1);
	printf(--1);
	printf(++1);
	printf(-10-10); 
	
	printf(xixi);
	printf(haha);
	printf(0);
	printf(hugao);
	printf(maogao);
	printf("history_process",history_process);
	printf('s'); 
}

int min(int a,int b) {
	if(a<b) return (a);
	return (b);
}

int max(int a,int b) {
	if(a>b) return (a);
	return (b);
}

int query(int l,int r,int type,int pos,int ll,int rr) {
	int flg,ans1,ans2;
	
	if(ll >= l) if(rr <= r) {
		if(type == 0) return(maxval[pos]);
		if(type == 1) return(minval[pos]);
		if(type == 2) return(sum[pos]);
		return(0);
	}
	flg=0;
	if(ll > r) flg=1;
	if(rr < l) flg=1;
	if(flg) {
		switch(type) {
			case 0: return(-2147483647);
			case 1: return (2147483647);
			case 2: return(0);
		}
		return (0);
	}
	
	switch(type) {
		case 0: return (max(query(l,r,type,pos*2,ll,(ll+rr)/2),query(l,r,type,pos*2+1,(ll+rr)/2+1,rr)));
		case 1: return (min(query(l,r,type,pos*2,ll,(ll+rr)/2),query(l,r,type,pos*2+1,(ll+rr)/2+1,rr)));
		case 2: return (query(l,r,type,pos*2,ll,(ll+rr)/2)+query(l,r,type,pos*2+1,(ll+rr)/2+1,rr));
	}
	return(0);
}


void build(int l,int r,int pos) {
	
	if(l==r) {
		maxval[pos] = data[l];
		minval[pos] = data[l];
		sum[pos]=data[l];
		return;
	}
	
	build(l,(l+r)/2,pos*2);
	build((l+r)/2+1,r,pos*2+1); 
	
	maxval[pos] = max(maxval[pos*2],maxval[pos*2+1]);
	minval[pos] = min(minval[pos*2],minval[pos*2+1]);
	sum[pos] = sum[pos*2] + sum[pos*2+1];
}



void echo() {
	char x;
	int i;
	i = 0;
	do{
		scanf(x);
		if(x=='0') return;
		printf(x);
		printf(x+2);
		i = i + 1;
	}while(1)
}

void fibo() {
	int tmp[4],ans[4],n,t[4];
	int i,j,k,s;
	s = 0;
	do{
		tmp[0]=0; tmp[1]=1; tmp[2]=1; tmp[3]=tmp[2];
		ans[0]=0; ans[1]=1; ans[2]=1; ans[3]=0;
		
		printf("Please input n, and the program will work out the n-th element of the fibonacci sequence:");
		printf("Input 0 to quit this test.");
		scanf(n);
		if(n==0) return;
		s = 0;
		do{
			if(n-n/2*2) {
				i = 0;
				do{
					j = 0;
					do{
						t[i*2+j]=0;
						k = 0;
						do{
							t[i*2+j]=t[i*2+j]+ans[i*2+k]*tmp[k*2+j];
							t[i*2+j]=t[i*2+j]-t[i*2+j]/10000*10000;
							k = k + 1;
						}while(k<2)
						j = j + 1;
					}while(j<2)

					i = i+1;
				}while(i<2)
				i = 0;
				do{
					ans[i]=t[i];
					i = i + 1;
				}while(i<4)
			}
			
			i = 0;
			do{
				j = 0;
				do{
					t[i*2+j]=0;
					k = 0;
					do{
						t[i*2+j]=t[i*2+j]+tmp[i*2+k]*tmp[k*2+j];
						t[i*2+j]=t[i*2+j]-t[i*2+j]/10000*10000;
						k = k + 1;
					}while(k < 2)
					j= j+1;
				}while(j<2)			
				i = i + 1;
			}while(i<2)
			i = 0;
			do{
				
				tmp[i]=t[i];
				i = i +1;
			}while(i<4)
			n=n/2;
			s = s +1;
		}while(n)
		printf("The result is: ", ans[1]);
		s = s+1;
	}while(1)
}


void main() {
	int i,j;
	int a,b,flg,x;
	int l,r,type;
	
	printf("The program first orzs libo to gain some rp and increase the probability of passing the test.");
	i = 0;
	do{
		printf("orzlibo ", i);
		i = i + 1;
	}while(i<10)
	printf(_o);
	printf(_r);
	printf(_z);
	printf(_l);
	printf(_i);
	printf(_b);
	printf(_o);
	
	flg=1;
	i = 0;
	do{
		printf("please input 2 integers, and the program will work out their gcd.");
		printf("input 2 zeros to quit the loop.");
		scanf(a,b);
		if(a==0) if(b==0) flg=0;
		if(flg) printf("the gcd is: ",gcd(a,b));
		i = i + 1;
	}while(flg)

	
	printf("Now, let's do some math problems for fun.");
	fibo();
	
	printf("Well, let's run another segment tree test to verify its correctness.");
	printf("Please input the length of the sequence:");
	scanf(n);
	
	i = 1;
	do{
		printf("Please input the ", i);
		printf("-th number: ");
		scanf(x);
		data[i]=x;
		i = i + 1;
	}while(i<=n)
	
	printf("The numbers you just inputed are:");
	i = 1;
	do{
		printf("The ", i);
		printf("-th: ",data[i]);
		i = i+  1;
	}while(i<=n)

	
	printf("Building segment tree...");
	build(1,n,1);
	printf("The tree is succesfully built! Now it can respond to your query requests.");
	flg=1;
	i = 0;
	do{
		printf("Please input 3 numbers, l, r, and type, type=-1 means exit:");
		scanf(l,r,type);
		if(type<0) flg=0;
		if(type>=3) flg=0;
		if(flg) {
			printf("Your query result is ",query(l,r,type,1,1,n));
		}
		i = i +1;
	}while(flg)

	
	printf("Echo testing...");
	echo();
	
	weird_expressions_test();
	
	calculate_e();
	
	printf("Quick sort test:");
	printf("Please input the number of integers:");
	scanf(m);
	i = 1;
	do{
		printf("Please input the ",i);
		printf("-th number:");
		scanf(x);
		qsort_data[i]=x;
		i = i+1;
	}while(i<=m)

	qsort(1,m);
	printf("The result after sorting is:");
	i = 1;
	do{
		printf(qsort_data[i]);
		i = i + 1;
	}while(i<=m)
}
