package lab1;

import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

//implement Iterative Deepening with DLS

	/*Pseudo Code:
		
		// Returns true if target is reachable from
		// src within max_depth
		bool IDDFS(src, target, max_depth)
		    for limit from 0 to max_depth
		       if DLS(src, target, limit) == true
		           return true
		    return false   

		bool DLS(src, target, limit)
		    if (src == target)
		        return true;

		    // If reached the maximum depth, 
		    // stop recursing.
		    if (limit <= 0)
		        return false;   

		    foreach adjacent i of src
		        if DLS(i, target, limit-1)             
		            return true

		    return false
*/
public class IDDLSolver {
	static long start_time = (long) 0;
	static long bound;
	public long total_time = (long) 0;
	public IDDLSolver(long b){
		bound = b;
		start_time = System.currentTimeMillis();
	}
	public boolean IDDLS(State root){
		int i = 0;
		for(i = 0; ; i++){
			if(TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - start_time) > bound){
				total_time = bound;
				return false;
			}
			if(DLS(root,i)){
				System.out.println("Found solution at depth: " + i);
				total_time = TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - start_time);
				return true;
			}else{
				if(!(total_time == 0)){
					return false;
				}
			}
		}
	}

	public boolean DLS(State current, int depth) {
		//System.out.println(current.getOp() != null ? current.getOp() + " D: " + depth : "no Parent");
		//int i = 0;
		if(current.isGoal()){
			
			return true;
		}
		if(depth <= 0){
			return false;
		}else if(TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - start_time) > bound){
			total_time = bound;
			return false;
		}
		
		ArrayList<Op> operations = current.generatePossibleMoves().getPossibleOperations();
		for(Op op: operations){
			State s = new State(current);
			s.setOp(op.constructOperation());
			s.setParent(current);
			s.makeMove(op);
			s.draw();
			if(DLS(s,depth-1)){
				System.out.println(op.constructOperation() + " ");
				
				return true;
			}
		}
		return false;
	}
	
	public static void main(String[] args){
		State s = new State();
		s.initilizeState("AABO..P.BO..PXXO..PQQQ...........RRR");
		IDDLSolver solver = new IDDLSolver(100000000);
		s.show();
		
		System.out.println(solver.IDDLS(s));
		System.out.println("finished in: " + solver.total_time/1000000.0 + " sec.");
	}

}
