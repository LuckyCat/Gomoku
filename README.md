# GomokuForInlusion
Five in a row

Project Architecture
	I use Unreal Engine 4 standart class hierarchy and classes workflow. Because it will be flexible for future project extension / development. For example network support implementation, new moves etc.
	
	UGomokuControllerInterface 
		GomokuPlayerController - Player controller
		GomokuAIController - Implements gomoku AI strategy logic. Can be extended from C++ or Blueprints for custom implementations.
		
	GomokuPawn - APawn Extendeble in blueprints. Controls visual representation of player moves
	GomokuBoard - Main AActor that implements all board mechanics
	GomokuMove - its an UObject that represent player move that applies to GomokuBoard. It can be extended in C++/Blueprints for adding new features like "black magic swap"
	
Additional Comments
	Instanced static meshes are used instead of bunch of single static meshes to boost the performance.
	
UI/Keyboard/Mouse
	Mouse scroll to Zoom camera
	Hold right mouse button to rotate camera
	Left button - to put token
	