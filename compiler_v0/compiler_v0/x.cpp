int triangle(int m,int n) {
	int x;
	int y;
	int column;
	int line;
	{
		x = 1;
		y = 0;
		line = 1;
		column = 1;
		print y;
		while (line<m)
			do
			{
				column = 1;
				while (column <= line)
					do
					{
						print x;
						column = column + 1
					};
				print y;
				line = line + 1
			};
		return y
	}
}
void main(int t) {
	int x;
	{
		x = triangle(10,1)
	}
}
#