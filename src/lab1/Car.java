package lab1;

public class Car {
	char name;
	boolean direction;
	Position head; //false for vertical and true for horizontal
	int size;
	
	public Car(int x, int y,char c, boolean direc){
		this.head = new Position(x,y);
		this.direction = (c == 'A' || c == 'B' || c == 'C' || c == 'X')? true:false;
		this.size = (c <= 'K' || c == 'X')? 2 : 3;  
		this.name = c;
		this.direction = direc;
	} 
	
	public int get_size(){
		return this.size;
	}
	
	public Position getPosition(){
		return head;
	}
	
	public void updatePosition(int _x,int _y){
		this.head.x = _x;
		this.head.y = _y;
	}
	
	public void setDir(boolean b){
		this.direction = b;
	}
	
	public boolean isVertical(){
		return direction == false;
	}
	
	public class Position{
		public Position(int x2, int y2) {
			this.x = x2;
			this.y = y2;
		}
		public int x;
		public int y;
	}
	 
	public String status(){
		return name +", " + (direction==false? "Vertical":"Horizontal") + ", " + size + ", " +name + ", at :("+(head.x+1) + "," + (head.y+1)+");";
	}

}
