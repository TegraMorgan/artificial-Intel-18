package lab1;

/*import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;*/
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;


public class State implements Comparable<State>,Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private char[][] board;
	Map<String,Car> cars = new HashMap<>();
	String carList = "";
	ArrayList<Op> nextStates = new ArrayList<>();
	private double h_value;
	private State parent;
	private String op;
	private Op opp;
	public String disc;
	boolean visited = false;
	int g = -1;
	//public int state_id;
	
	public State(){
		board = new char[6][6];
	}

	public State(State other){
		this.board = new char[6][6];
		this.carList = other.carList;
		this.cars = new HashMap<>(other.cars.size());
		for (Entry<String, Car> c : other.cars.entrySet()) {
			this.cars.put(c.getKey(), new Car(c.getValue()));
		}
		this.setHValue(other.getHValue());
		//this.nextStates = new ArrayList<>(other.nextStates);
		this.setParent(other.getParent());
		this.setOp(other.getOp());
	}
	
	public char[][] getBoard(){
		return this.board;
	}
	
	//TODO [test this]
	public void initilizeState(String disc){
		final int ROW = 6;
		final int COL = 6;
		boolean direction = false;;
		for(int i = 0, k = 0; i < ROW; i++){
			for(int j = 0; j < COL; j++){
				char car = disc.charAt(k++);
				board[i][j] = car;
				if(car == '.'){
					continue;
				}else{
					if(carList.indexOf(car) == -1){
						carList += car;
						if(j < COL -1 && k <= disc.length() -1 ){
							if(disc.charAt(k) == car){
								direction = true;
							}
						}
						cars.put(car+"",new Car(i,j,car,direction));
						direction = false;
					}
				}
				/***/
				
			}
		}
		compress();
	}
	
	public void draw(){
		boolean goal = this.isGoal();
		final int ROW = 6;
		final int COL = 6;
		for (Entry<String, Car> c : this.cars.entrySet()) {
			if(goal && c.getKey().equals("X"))
				continue;
			int x = c.getValue().getPosition().x,y = c.getValue().getPosition().y;
			char name = c.getValue().name;
			this.board[x][y] = name;
			//System.out.println("KKKK      " + name);
			if(c.getValue().isVertical()){
				this.board[x+1][y] = name;
				if(c.getValue().get_size() == 3)
					this.board[x+2][y] = name;
				
			}else{
				this.board[x][y+1] = name;
				if(c.getValue().get_size() == 3)
					this.board[x][y+2] = name;
			}
		    
		}
		for(int i = 0; i < ROW; i++){
			for(int j = 0; j < COL; j++){
				if(this.board[i][j] < 'A'){
					this.board[i][j] = '#';
				}
			}
		}
	}

	public void show(){
		final int ROW = 6;
		final int COL = 6;
		for(int i = 0; i < ROW; i++){
			for(int j = 0; j < COL; j++){
				System.out.print(board[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println();
		/*for (Entry<String, Car> entry : this.cars.entrySet()) {
		    
		    System.out.println(entry.getValue().status());
		    
		}*/
		
	}
	
	/**
	 * Returns null upon reaching the goal state
	 */
	public State generatePossibleMoves(){
		if(this.isGoal()){
			this.nextStates = null;
			return this;
		}
		if(!this.nextStates.isEmpty()){
			this.nextStates.clear();
		}
		for (Entry<String, Car> c : this.cars.entrySet()) {
			if(!c.getValue().isVertical()){
				int leftRange = canMoveLeft(c.getValue());
				int rightRange = canMoveRight(c.getValue());
				if(rightRange != 0){
					for(int k = 1; k <= rightRange; k++){
						nextStates.add(new Op(Move.RIGHT, k, c.getValue().name));
					}
					if(c.getValue().name == 'X'){
						if(c.getValue().getPosition().y + rightRange == 4){
							nextStates.add(new Op(Move.RIGHT,rightRange + 2,'X'));
						}
					}
				}
				if(leftRange != 0){
					for(int k = 1; k <= leftRange; k++){
						nextStates.add(new Op(Move.LEFT, k, c.getValue().name));
					}
				}
			}else{
				int upRange = canMoveUp(c.getValue());
				int downRange = canMoveDown(c.getValue());
				if(upRange != 0){
					for(int k = 1; k <= upRange; k++){
						nextStates.add(new Op(Move.UP, k, c.getValue().name));
					}
				}
				if(downRange != 0){
					for(int k = 1; k <= downRange; k++){
						nextStates.add(new Op(Move.DOWN, k, c.getValue().name));
					}
				}
			}
		
		}
		return this;
		
	}
	
	private int canMoveDown(Car c) {
		final int ROW = 6;	
		int bound = c.getPosition().x + c.get_size(),count = 0;
		for(int i = bound,j = c.getPosition().y; i < ROW; i++ ){
			if(this.board[i][j] >= 'A' && this.board[i][j] <= 'Z'){
				break;
			}else{
				count++;
			}
		}
		return count;
	}

	private int canMoveUp(Car c) {
		int bound = c.getPosition().x - 1,count = 0;
		for(int i = bound,j = c.getPosition().y; i >= 0; i-- ){
			if(this.board[i][j] >= 'A' && this.board[i][j] <= 'Z'){
				break;
			}else{
				count++;
			}
		}
		return count;
	}

	private int canMoveRight(Car c) {
		final int COL = 6;
		int bound = c.getPosition().y + c.get_size(),count = 0;
		
		for(int i = c.getPosition().x,j = bound; j < COL; j++ ){
			
			if(this.board[i][j] >= 'A' && this.board[i][j] <= 'Z'){
				break;
			}else{
				count++;
			}
		}
		return count;
	}

	private int canMoveLeft(Car c) {
		int bound = c.getPosition().y - 1,count = 0;
		
		for(int i = c.getPosition().x,j = bound; j >= 0; j-- ){
			if(this.board[i][j] >= 'A' && this.board[i][j] <= 'Z'){
				break;
			}else{
				count++;
			}
		}
		return count;
	}
	
	public void showNextStates(){
		if(this.isGoal()){
			System.out.println("\n\n################################\n\nThis is the goal state, Nothing to be done.\n\n################################\n\n");
			return;
		}
		this.generatePossibleMoves();
		for(Op o: nextStates){
			System.out.println(o.constructOperation());
		}
	}
	
	public void setHValue(double h2_value){
		this.h_value = h2_value;
	}
	
	public double getHValue(){
		return this.h_value;
	}
	
	public void makeMove(Op op){
		Car movingCar = this.cars.get(op.get_car()+"");
		int x = movingCar.getPosition().x,
				y = movingCar.getPosition().y,
					size = movingCar.get_size();
		if(movingCar.isVertical()){
			for(int i = x,j = y; i< size + x; i++){
				this.board[i][j] = '.';
			}
			if(op.get_move() == Move.DOWN){
				for(int i = x + op.get_count(), j = y; i < x + op.get_count() + size; i++ ){
					this.board[i][j] = movingCar.name;
				}
				movingCar.getPosition().x = x + op.get_count();
			}else{
				for(int i = x - op.get_count(), j = y; i < x - op.get_count() + size; i++ ){
					this.board[i][j] = movingCar.name;
				}
				movingCar.getPosition().x = x - op.get_count();
			}
			
		}else{
			for(int i = x,j = y; j< size + y; j++){
				this.board[i][j] = '.';
			}
			if(op.get_car() == 'X'){
				if(op.get_count() + cars.get('X' + "").getPosition().y == 6 && op.get_move() == Move.RIGHT){
					//this is the goal state
					movingCar.getPosition().y = y + op.get_count();	
					return;
				}
			}
			if(op.get_move() == Move.RIGHT){
				
				for(int i = x, j = y + op.get_count(); j < y + op.get_count() + size; j++ ){
					if(j == y + op.get_count()){
						//System.out.println(y + " ## " + op.get_count() + " ## " + size + "^^^ Op: " + op.constructOperation() );
					}
					this.board[i][j] = movingCar.name;
				}
				movingCar.getPosition().y = y + op.get_count();
			}else{
				for(int i = x, j = y - op.get_count(); j < y - op.get_count() + size; j++ ){
					this.board[i][j] = movingCar.name;
				}
				movingCar.getPosition().y = y - op.get_count();
			}
		}
		this.nextStates.clear();
	}
	
	@Override
	public int compareTo(State other) {
		double h2_value = other.getHValue();
		if(this.h_value > h2_value)
			return 1;
		if(this.h_value < h2_value)
			return -1;
		return 0;
	}
	
	public boolean isGoal(){
		return this.cars.get('X' + "").getPosition().y == 6; 
	}
	
	public void setParent(State p){
		this.parent = p;
	}
	
	public State getParent(){
		return this.parent;
	}
	
	public void setOp(String op2){
		this.op = op2;
	}
	
	public void setOpp(Op op){
		this.opp = op;
	}
	
	public String getOp(){
		return this.op;
	}
	
	public ArrayList<Op> getPossibleOperations(){
		return this.nextStates;
	}
	
	
	public void compress(){
		String quatized_state = "";
		for (Entry<String, Car> c : this.cars.entrySet()) {
			quatized_state += c.getValue().compress();
		}
		this.disc = quatized_state;
	}
	
	public static void main(String[] args){
		State s = new State();
		s.initilizeState(".............XXO...AAO.P.B.O.P.BCC.P");
		s.generatePossibleMoves();
		s.show();
		s.showNextStates();
		System.out.println(s.carList);
		/*State ss = new State();
		ss.initilizeState("AA...........XX.....................");
		*/
		/*s.show();
		s.showNextStates();
		System.out.println();
		s.makeMove(s.nextStates.get(2));
		s.show();
		
		s.showNextStates();
		s.show();*/
		//.out.println(s.compress().equals(ss.compress()));
		
	}

	public Op getOpp() {
		// TODO Auto-generated method stub
		return this.opp;
	}
	
	
}
