/////////////////////////////////////////////////////////////////////////////////////////////////
//Line style constants
typedef enum LineType {Solid, Dash, Dot, DashDot, DashDotDot, Null, XYStep, YXStep, Bars, Stick};

/////////////////////////////////////////////////////////////////////////////////////////////////
//Enum to describe symbol style constants
typedef enum SymbolType {Dots, Rectangles, Diamonds,	Asterisk, 
						 DownTriangles, UpTriangles, LeftTriangles, RightTriangles};

typedef enum CursorMode {Fixed, Floating, Snap};
typedef enum Crosshair  {XY, X, Y};