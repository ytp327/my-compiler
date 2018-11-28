int Fibo(int m) {
	int x;
	int y;
	{
		x = 1;
		y = 2;
		if (m:=x or m:= y)
		then
		{
			return x
		};
		x = Fibo(m - 1);
		y = Fibo(m - 2); 
		x = x + y;
		return x
	}
}
void main(int t) {
	int x;
	{
		x=Fibo(8);
		print x
	}
}
#