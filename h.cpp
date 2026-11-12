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

	int x = std::min({aP0.x, aP1.x, aP2.x});
	int max_x = std::max({aP0.x, aP1.x, aP2.x});
	//check x is the lowest value.
	if(x <= 0) {x =0;}
	//finally check that we are within the width
	int width = aSurface.get_width() -1;
	if (x >= width) {x = width;}
	//check max_x is the largest value

	if(max_x >= width) {max_x = width;}
	//check y is the lowest value
	int y = std::min({aP0.y, aP1.y, aP2.y});
	int max_y = std::max({aP0.y, aP1.y, aP2.y});
	int height = aSurface.get_height() -1;
	if(y > height) {y = height;}
	if(y <= 0) {y = 1;}
	//check max_y is the largest value
	if(max_y > height) {max_y = height;}
	
	Vec2f u = operator-(aP2, aP0);
	Vec2f v = operator-(aP1, aP0);
	float area = abs(( u.x * v.y) - (v.x *u.y)) /2; //area of P0, P1, P2
	ColorU8_sRGB colour;
	float area_A;
	float area_B;
	float area_C;
	Vec2f point;
	Vec2f p0p; //p0 and point vector
	Vec2f p2p;
	Vec2f p1p;
	float alpha;
	float beta;
	float gamma;

	//now we pick a point, start with (x, y)
	//make 3 mini triangles. find areas. compute alpha, beta, gamma - a, b, c
	//check conditions



	for (int i=x; i < max_x; i++) {
		for (int j=y; j < max_y; j++ ) {

			//we want to check every point in the triange and set that pixel to a colour
			
			point = {(float(i)+0.5f), (float(j)+0.5f)};

			//printf("point: %s\n", point);
			p0p = operator-(aP0, point);
			p2p = operator-(aP2, point);
			area_A = abs(( p0p.x * p2p.y) - (p2p.x *p0p.y)) /2;
			alpha = area_A / area;			
			//printf("alpha: %f\n", alpha);



			if( alpha <= 1 && alpha >= 0) { 
				alpha = area_A / area;	
			 	p1p = operator-(aP1, point);
				area_B =abs(( p0p.x * p1p.y) - (p1p.x *p0p.y)) /2;
				beta = area_B / area;
			//	printf("beta: %f\n", beta);

				if (beta <= 1 && beta >= 0) { 
					area_C = abs(( p1p.x * p2p.y) - (p2p.x *p1p.y)) /2;
					gamma = area_C / area;
					//printf("gamma: %f\n", gamma);

					
	

					if (gamma <= 1 && gamma >= 0) { 
						//printf("total: %f", total);
						//for floating point rounding errors. 
						if (alpha+beta+gamma-1 < 0.000001 ) {
							//point is in triangle so we want to draw it. 

							colour.r = linear_to_srgb(alpha * aC0.r + beta * aC1.r + gamma * aC2.r);
							colour.b = linear_to_srgb(alpha * aC0.b + beta * aC1.b + gamma * aC2.b);
							colour.g = linear_to_srgb(alpha * aC0.g + beta * aC1.g + gamma * aC2.g);
			 				aSurface.set_pixel_srgb(i, j, colour);


						}
					}
				}
			}

			
			

		}
	}


}