int main()
{
	//y' = y + x
	auto f = [](diff::vector2f p) 
	{ 
		return p.x + p.y;
	};

	float x0 = 0.0f, y0 = 0;
	float a = 0.0f, b = 1, h = 0.2f;

	std::vector<diff::vector2f> ps = diff::methodRungeKuttSecondOrder(f, y0, a, b, h);

	// Вывод результата (набор точек).
	for (size_t i = 0; i < ps.size(); i++)
	{
		std::cout << "(" << ps[i].x << "; " << ps[i].y << ") ";
	}

	return 0;
}
