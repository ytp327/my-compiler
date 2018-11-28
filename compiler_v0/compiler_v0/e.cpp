int triangle(int m) {
	int y;
	int column;
	int line;
	{
		y = 0;
		line = 1;
		column = 1;
		print y;
		while(line<m)
		do 
		{
			column = 1;
			while(column<=line)
			do
			{
				print line;
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
		x = triangle(5)
	}
}
#