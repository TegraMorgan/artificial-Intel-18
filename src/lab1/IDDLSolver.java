package lab1;

import java.util.ArrayList;

//TODO implement Iterative Deepening with DLS

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
	
	public boolean IDDLS(State root, int depth){
		for(int i = 0; i <= depth; i++){
			//root.show();
			if(DLS(root,i)){
				System.out.println("Found solution at depth: " + i);
				return true;
			}
		}
		System.out.println("Solution wasn't found within depth: " + depth);
		return false;
	}

	public boolean DLS(State current, int depth) {
		//System.out.println(current.getOp() != null ? current.getOp() + " D: " + depth : "no Parent");
		//int i = 0;
		if(current.isGoal()){
			
			return true;
		}
		if(depth <= 0){
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
				System.out.println(op.constructOperation() + " $$");
				
				return true;
			}
		}
		return false;
	}
	
	public static void main(String[] args){
		State s = new State();
		s.initilizeState("A..OOOA..B.PXX.BCPQQQ.CP..D.EEFFDGG.");
		IDDLSolver solver = new IDDLSolver();
		s.show();
		System.out.println(solver.IDDLS(s, 8));
	}

}