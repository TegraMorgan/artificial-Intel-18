package lab1;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/* 	(1) Put the start node s on a list, called OPEN, of unexpanded nodes. 
 * 			Calculate f(s) and associate its value with node s.
   	(2) If OPEN is empty, exit with failure, no solution exists.
	(3) Select from OPEN a node i at which f is minimum. If several nodes qualify, 
				choose a goal node if there is one, otherwise choose among them arbitrarily.
	(4) Remove node i from OPEN and place it on a list called CLOSED, of expanded nodes.
	(5) If i is a goal node, exit with success; a solution has been found.
	(6) Expand node i, creating nodes for all of its successors. For every successor node j of i:
	
		a. Calculate f(j)
		
		b. If j is neither in OPEN nor in CLOSED, then add it to OPEN with its f value. 
				Attach a pointer from j back to its predecessor i 
					(in order to track back a solution path once a goal node is found).
					
		c. If j was already on either OPEN or CLOSED, compare the f value just calculated for j with 
						the value previously associated with the node. If the new value is lower:
			i.   Substitute it for the old value.
			ii.  Point j back to i instead of to its previously found predecessor.
			iii. If node j was on the CLOSED list, move it back to OPEN
			
	(7) Go to (2)
*/
//TODO (1) - implement A* algorithm using the above pseudo-code
//TODO (2) - implement 2 heuristics functions within this context (At least 2)

public class A_StarSolver {	
	static FibonacciHeap<State> OPEN_LIST = new FibonacciHeap<>();
	static Map<Integer,State > CLOSED_LIST = new HashMap<>();
	private static int id = 1;
	private static double heuristicsSample = 100000;
	State root;
	
	public A_StarSolver(State initialState){
		this.root = initialState;
	}
	
	public static void assignID(State s){
		s.state_id = id;
		id++;
	}
	
	/**
	 * A* algorithm implementation
	 * @return
	 */
	public boolean solve(){
		calculate_Heuristics(root);
		OPEN_LIST.insert(new FibonacciHeapNode<State>(root, root.getHValue()), root.getHValue());
		
		//A* loop starts here
		while(!OPEN_LIST.isEmpty()){
			State min = getMin();
			assignID(min);
			CLOSED_LIST.put(min.state_id, min);
			if(min.isGoal()){
				System.out.println("Solution FOUND!");
				return true;
			}
			ArrayList<Op> operations = min.generatePossibleMoves().getPossibleOperations();
			for(Op op: operations){
				State s = new State(min);
				s.setOp(op.constructOperation());
				s.setParent(min);
				s.makeMove(op);
				s.draw();
				calculate_Heuristics(s);
				OPEN_LIST.insert(new FibonacciHeapNode<State>(s, s.getHValue()), s.getHValue());
			}
		}
		
		return false;
	}
	
	/**
	 * 
	 * 
	TODO - implement F(n) = g(n) + h(n). 
	#NOTE# h(n) needs to be ADMISSIBLE!
	
	this is a sample calculation ONLY!!
	*
	*
	**/
	private void calculate_Heuristics(State node) {
		node.setHValue(--A_StarSolver.heuristicsSample);
	}
	
	/**
	 * TODO - needs to be configured to choose goal state if it exists AND handling collisions
	 * @return
	 */
	public State getMin(){
		return OPEN_LIST.min().data;
	}
	public void remove_min(){
		OPEN_LIST.removeMin();
	}
	
	
	public static void main(String args[]){
		
	}

}

