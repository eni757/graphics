void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	// TODO: your implementation goes here

	//draw triangle
	//find lowest x coordinate, go in lines from one side to other. check if pixel is in the triangle.
	//check pixel is in triangle using barry coords. 
	
	//cross product - find area of big triangle
	//find alpha, beta, gamma (sub traingles) a, b,y 
	//check that a, b, y properties hold so between 0 and 1. and a+b+y=1 are true. 
	//if true, is in the triangle, move onto pixels. 

	//based on how far pixel is from each of the points, set it to a colour. 
	//rinse and repeat for all pixel

	//draw_line_solid(aSurface, aP0, aP1, aC0);
	//draw_line_solid(aSurface, aP1, aP2, aC1);
	//draw_line_solid(aSurface, aP2, aP0, aC2);

	int x = aP0.x;
	int max_x = aP0.x;
	//check x is the lowest value.
	if (x > aP1.x) {x = aP1.x;}
	if(x > aP2.x) {x = aP2.x;}
	if(x < 0) {x =0;}
	//finally check that we are within the width
	int width = aSurface.get_width() -1;
	if (x > width) {x = width;}
	//check max_x is the largest value
	if(max_x < aP1.x) {max_x = aP1.x;}
	if(max_x < aP2.x) {max_x = aP2.x;}
	if(max_x > width) {max_x = width;}
	//check y is the lowest value
	int y = aP0.y;
	int max_y = aP0.y;
	int height = aSurface.get_height() -1;
	if (y > aP1.y) {y = aP1.y;}
	if (y > aP2.y) {y = aP2.y;}
	if(y > height) {y = height;}
	if(y < 0) {y = 0;}
	//check max_y is the largest value
	if(max_y < aP1.y) {max_y = aP1.y;}
	if(max_y < aP2.y) {max_y = aP2.y;}
	if(max_y > height) {max_y = height;}

	std::printf("x: %d\n", x);
	std::printf("y: %d\n", y);
	std::printf("width: %d\n", width);
	std:: printf("height: %d\n", height);
	//need to do u x v to find area
	
	Vec2f u = operator-(aP2, aP0);
	Vec2f v = operator-(aP1, aP0);

	//std::printf("x: %f  y: %f\n"u.x, u.y);
	std::printf("v x: %f, y: %f\n",v.x, v.y);
	float area = abs(( u.x * v.y) - (v.x *u.y)) /2; //area of P0, P1, P2
	ColorU8_sRGB colour;
	float area_A;
	float area_B;
	float area_C;
	Vec2f point;
	Vec2f a1;
	Vec2f a2;
	Vec2f b1;
	Vec2f b2;
	Vec2f c1;
	Vec2f c2;
	float alpha;
	float beta;
	float gamma;
	float total;

	//now we pick a point, start with (x, y)
	//make 3 mini triangles. find areas. compute alpha, beta, gamma - a, b, c
	//check conditions



	for (int i=x; i < max_x; i++) {
		for (int j=y; j < max_y; j++ ) {

			//we want to check every point in the triange and set that pixel to a colour
			
			point = {float(i), float(j)};

			//printf("point: %s\n", point);
			a1 = operator-(aP0, point);
			a2 = operator-(aP2, point);
			area_A = abs(( a1.x * a2.y) - (a2.x *a1.y)) /2;
			alpha = area_A / area;			
			//printf("alpha: %f\n", alpha);



			if( alpha <= 1 && alpha >= 0) { 
				b1 = operator-(aP0, point);
			 	b2 = operator-(aP1, point);
				area_B =abs(( b1.x * b2.y) - (b2.x *b1.y)) /2;
				beta = area_B / area;
			//	printf("beta: %f\n", beta);

				if (beta <= 1 && beta >= 0) { 
					c1 = operator-(aP1, point);
			 		c2 = operator-(aP2, point);
					area_C = abs(( c1.x * c2.y) - (c2.x *c1.y)) /2;
					gamma = area_C / area;
					//printf("gamma: %f\n", gamma);

					
	

					if (gamma <= 1 && gamma >= 0) { 
						total = alpha + beta + gamma;
						//printf("total: %f", total);

						if (total == 1 ) {
							//point is in triangle so we want to draw it. 
							colour = linear_to_srgb(aC0);
			 				aSurface.set_pixel_srgb(x, y, colour);


						}
					}
				}
			}

			
			

		}
	}


}