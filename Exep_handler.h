int handle(double begin_y, double end_y, double c, double c1){

	if(begin_y >= c1 || begin_y <= c){

		return 1;
	}
	if(end_y >= c1 || end_y <= c){

		return 1;
	}

	return 0;
}