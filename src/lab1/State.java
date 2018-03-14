package lab1;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;


public class State implements Comparable<State>{
	
	final int ROW = 6;
	final int COL = 6;
	final boolean VERTICAL = false;
	final boolean HORIZONTAL = true;
	private char[][] board;
	Map<String,Car> cars = new HashMap<>();
	String carList = "";
	int redCarIndex;
	ArrayList<Op> nextStates = new ArrayList<>();
	private double h_value;
	
	public State(){
		board = new char[6][6];
	}
	
	public void initilizeState(String disc){
		boolean direction = false;;
		for(int i = 0, k = 0; i < ROW; i++){
			for(int j = 0; j < COL; j++){
				char car = disc.charAt(k++);
				board[i][j] = car;
				if(car == '.'){
					continue;
				}else{
					if(carList.indexOf(car) == -1){
						if(car == 'X'){
							redCarIndex = carList.length();
						}
						carList += car;
						if(j < COL -1 && k < disc.length() -1 ){
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
	}
	
	public void draw(){
		for (Entry<String, Car> c : this.cars.entrySet()) {
			int x = c.getValue().getPosition().x,y = c.getValue().getPosition().y;
			char name = c.getValue().name;
			this.board[x][y] = name;
			
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
		for(int i = 0; i < ROW; i++){
			for(int j = 0; j < COL; j++){
				System.out.print(board[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println();
		for (Entry<String, Car> entry : this.cars.entrySet()) {
		    
		    System.out.println(entry.getValue().status());
		    
		}
		
	}
	
	/**
	 * Returns null upon reaching the goal state
	 */
	public void generatePossibleMoves(){
		if(this.redCarIndex == 6){
			this.nextStates = null;
		}
		for (Entry<String, Car> c : this.cars.entrySet()) {
			if(!c.getValue().isVertical()){
				int leftRange = canMoveLeft(c.getValue());
				int rightRange = canMoveRight(c.getValue());
				if(rightRange != 0){
					for(int k = 1; k <= rightRange; k++){
						nextStates.add(new Op(Move.RIGHT, k, c.getValue().name));
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
		
	}
	
	private int canMoveDown(Car c) {
		int bound = c.getPosition().x + c.get_size(),count = 0;
		for(int i = bound,j = c.getPosition().y; i < ROW; i++ ){
			if(this.board[i][j] != '.'){
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
			if(this.board[i][j] != '.'){
				break;
			}else{
				count++;
			}
		}
		return count;
	}

	private int canMoveRight(Car c) {
		int bound = c.getPosition().y + c.get_size(),count = 0;
		
		for(int i = c.getPosition().x,j = bound; j < COL; j++ ){
			
			if(this.board[i][j] != '.'){
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
			if(this.board[i][j] != '.'){
				break;
			}else{
				count++;
			}
		}
		return count;
	}
	
	public void showNextStates(){
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
	//TODO: implement the makeMove method
	//It takes an Op object and changes the state
	
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
			if(op.get_move() == Move.RIGHT){
				for(int i = x, j = y + op.get_count(); j < y + op.get_count() + size; j++ ){
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

	public static void main(String[] args){
		State s = new State();
		s.initilizeState("OAAP..O..P..OXXP....BQQQ..B..E..RRRE");
		s.show();
		s.showNextStates();
		System.out.println();
		s.makeMove(s.nextStates.get(1));
		s.show();
		
	}
	
	
}
