#include<iostream>
int main()
{
	int s , t ,u;
	s = 0;
	t = 100;
	u = 1;
	while(1){
		s = s + u;
		u = u + 1;
	}
	if(s < 100){
		s = s *100;
	}
	else{
		s = s /100;
	}
	return 0;	
}