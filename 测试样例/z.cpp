int hanoi(int n,int counter)
{
	int x;
	{
		x = 1;
		if (x := n)
		then
		{
			counter = counter + 1;
			return counter
		}
		else
		{
			counter=hanoi(n-1,counter);
			counter = counter + 1;
			counter=hanoi(n - 1, counter);
			return counter
		}
	}
}
void main(int x)
{
	int x;
	{
		x = hanoi(3, 0);
		print x
	}
}#