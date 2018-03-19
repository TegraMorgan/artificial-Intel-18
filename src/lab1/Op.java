package lab1;

import java.io.Serializable;

public class Op implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private int count;
	private Move move;
	private char car;
	
	public Op(Move direction, int numOfSteps, char name){
		this.count = numOfSteps;
		this.move = direction;
		this.car = name;
	}
	
	public int get_count(){
		return this.count;
	}
	
	public Move get_move(){
		return this.move;
	}
	
	public char get_car(){
		return car;
	}
	
	public String constructOperation(){
		String op = null;
		if(this.move == Move.DOWN){
			op = "D";
		}else if(move == Move.UP){
			op = "U";
		}else if(move == Move.RIGHT){
			op = "R";
		}else{
			op = "L";
		}
		return this.car+op+this.count;
	}
}
