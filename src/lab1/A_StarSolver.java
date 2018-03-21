package lab1;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

/***
 * 
 * TODO - test 12,17,24,33,36,38
 *
 */
/* 	(1) Put the start node s on a list, called OPEN, of unexpanded nodes. 
 * 			Calculate f(s) and associate its value with node s.
   	(2) If OPEN is empty, exit with failure, no solution exists.
	(3) Select from OPEN a node i at which f is minimum. If several nodes qualify, 
				choose a goal node if there is one, otherwise choose among them arbitrarily.
	(4) Remove node i from OPEN and place it on a list called CLOSED, of expanded nodes.
	(5) If i is a goal node, exit with success; a solution has been found.
	(6) Expand node i, creating nodes for all of its successors. For every successor node j of i:
	
		a. Calculate f(j)- DONE!!!!
		
		b. If j is neither in OPEN nor in CLOSED, then add it to OPEN with its f value. 
				Attach a pointer from j back to its predecessor i 
					(in order to track back a solution path once a goal node is found). DONE
					
		c. If j was already on either OPEN or CLOSED, compare the f value just calculated for j with 
						the value previously associated with the node. If the new value is lower:
			i.   Substitute it for the old value.
			ii.  Point j back to i instead of to its previously found predecessor.
			iii. If node j was on the CLOSED list, move it back to OPEN
			
	(7) Go to (2)
*/



public class A_StarSolver {	
	static FibonacciHeap<State> OPEN_LIST;
	static Map<String,FibonacciHeapNode<State> > OPEN_LIST_HELPER;
	static Map<String,FibonacciHeapNode<State>  > CLOSED_LIST;
	//private static int id = 1;
	State root;
	static int branching_factor;
	static int level;
	Stack<Op> ops = new Stack<Op>(); 
	public A_StarSolver(State initialState){
		this.root = initialState;
		OPEN_LIST = new FibonacciHeap<>();
		OPEN_LIST_HELPER = new HashMap<>();
		CLOSED_LIST = new HashMap<>();
		branching_factor = 0;
		level = 0;
	}

	/**
	 * A* algorithm implementation
	 * @return boolean
	 */
	static String voo = "";
	static int[] optimal_solutions = new int[40];
	public boolean solve(){
		root.setParent(null);
		root.g = 0;
		calculate_Heuristics(root);
		FibonacciHeapNode<State> rootHeapNode = new FibonacciHeapNode<State>(root, root.getHValue());
		OPEN_LIST.insert(rootHeapNode, root.getHValue());
		OPEN_LIST_HELPER.put(root.disc, rootHeapNode);
		
		//final long startTime = System.currentTimeMillis();
		while(!OPEN_LIST.isEmpty()){
			level++;
			FibonacciHeapNode<State> min = OPEN_LIST.min();
			min = remove_min();
			
			
				
				
			//DEBUG
			CLOSED_LIST.put(min.data.disc, min);
			if(min.data.isGoal()){
				System.out.println("Solution FOUND!");
				State path = min.data;
				//int ii = 0;
				while(path.getParent() != null){
					/*if(ii > 1 && path.getParent().getOp().equals(path.getOp())){
						System.out.println("##################");
						path.show();
						ii++;
					}*/
					ops.push(path.getOpp());
					path = path.getParent();
				}
				branching_factor *= level;
				return true;
			}
			/**
			 * @DEBUG
			 */
			if(min.data.getOpp()!= null){
				if(min.data.getOpp().constructOperation().equals("QR2")){
					System.out.println("//Debug: \nParent: ");
					min.data.getParent().show();
					System.out.println("Current: ");
					min.data.show();
					System.out.println("\nPossible moves: ");
					min.data.showNextStates();
					System.out.println("\nEND of pm\n");
				}
			}
			ArrayList<Op> operations = min.data.generatePossibleMoves().getPossibleOperations();
			for(Op op: operations){
				/*if(op.constructOperation().equals("QR2") && min.data.getParent().getOpp().constructOperation().equals("QR2")){
					
					System.out.println("$#$#$#$#$#$#$\n");
					min.data.show();
				
			}*/
				State s = new State(min.data);
				s.setOpp(op);
				s.compress();
				s.makeMove(op);				
				s.draw();
				s.compress();
				s.setParent(min.data);
				if(s.g < 0){
					s.g = min.data.g + 1;
				}
				calculate_Heuristics(s);
				
				//Checks if the state was opened/closed previously
				
				boolean opened = OPEN_LIST_HELPER.containsKey(s.disc)
						,closed = CLOSED_LIST.containsKey(s.disc);
				FibonacciHeapNode<State> stateHelper = null;
				if(closed || opened){
					
					//System.out.println(opened? "#opened#\n":"#closed#\n"+ "      lvl: " + level);
					stateHelper = (closed)? CLOSED_LIST.get(s.disc): OPEN_LIST_HELPER.get(s.disc);
					
					//stateHelper.data.show();
					if(s.getHValue() < stateHelper.key){
						//stateHelper.setHValue(s.getHValue());
						if(opened){
							//OPEN_LIST.decreaseKey(stateHelper, s.getHValue());
							OPEN_LIST.delete(stateHelper);
							OPEN_LIST_HELPER.remove(stateHelper);
							stateHelper.data.setHValue(s.getHValue());
							insert_opened(stateHelper);
						}else{
							//fbn equals to stateHelper
							FibonacciHeapNode<State> fbn = CLOSED_LIST.remove(s.disc);
							fbn.data.setHValue(s.getHValue());
							insert_opened(fbn);
							
						}
						//stateHelper.data.setParent(min.data);
					}
					//System.out.println("YEPPE");
					if(min.data.getOpp().constructOperation().equals("QR2")){
						if(op.constructOperation().equals("QR2")){
							System.out.println("here");
						}
					}
					continue;
				}
				branching_factor++;
				FibonacciHeapNode<State> new_fbn = new FibonacciHeapNode<State>(s, s.getHValue());
				insert_opened(new_fbn);
				/*if(TimeUnit.MILLISECONDS.toMinutes(System.currentTimeMillis() - startTime) > 1){
					System.out.println("SS");
					return false;
				}*/
				
				if(min.data.getOpp() != null && min.data.getOpp().constructOperation().equals("QR2")){
					if(op.constructOperation().equals("QR2")){
						System.out.println("here");
					}
				}
				
			}
		}
		return false;
	}
	
	public void insert_opened(FibonacciHeapNode<State> fbn) {
		OPEN_LIST.insert(fbn, fbn.data.getHValue());
		OPEN_LIST_HELPER.put(fbn.data.disc, fbn);
		
	}

	private void calculate_Heuristics(State node) {
		if(node.isGoal()){
			node.setHValue(-100.0);
		}else{
			double count = 0;
			char[][] board = node.getBoard();
			int CAR_LENGTH = 2;
			for(int i = node.cars.get("X").getPosition().y + CAR_LENGTH ; i < 6; i++){
				char c = board[2][i];
				if(c != 'X' && c >='A' && c <= 'Z'){
					count += 10;
				}
			}
			node.setHValue(count + node.g);
		}
	}
	
	public State getMin(){
		return OPEN_LIST.min().data;
	}
	
	public FibonacciHeapNode<State> remove_min(){
		FibonacciHeapNode<State> min = OPEN_LIST.removeMin();
		OPEN_LIST_HELPER.remove(min.data.disc);
		return min;
	}
	
	public static double nthroot(int n, double A) {
		return nthroot(n, A, .001);
	}
	
	public static double nthroot(int n, double A, double p) {
		if(A < 0) {
			System.err.println("A < 0");// we handle only real positive numbers
			return -1;
		} else if(A == 0) {
			return 0;
		}
		double x_prev = A;
		double x = A / n;  // starting "guessed" value...
		while(Math.abs(x - x_prev) > p) {
			x_prev = x;
			x = ((n - 1.0) * x + A / Math.pow(x, n - 1.0)) / n;
		}
		return x;
	}
	
	public static void main(String[] args){
		
		try(BufferedReader br = new BufferedReader(new FileReader("states2.txt"))) {
			int k = 0;
		    for(String line; (line = br.readLine()) != null; ) {
		    	//System.out.println(line);
		        //automation(line);
		    	line = line.trim();
		    	int shouldBe = 0;
		    	if(line.startsWith("Soln")){
		    		//System.out.println(line);
		    		line = line.substring(6);
		    		shouldBe += line.split(" ").length;
		    		while(!line.endsWith(".")){
		    			line = br.readLine().trim();
		    			shouldBe += line.split(" ").length;
		    		}
		    		A_StarSolver.optimal_solutions[k++] = shouldBe-1;
		    	}
		    	
		    }
		    BufferedReader br2 = new BufferedReader(new FileReader("states.txt"));
		    for(String line; (line = br2.readLine()) != null; ) {
		    	//System.out.println(line);
		        automation(line);
		    	
		    }
		    // line is not visible here.
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		/*State s = new State();
		s.initilizeState("ABB..OA.P..OXXP..O..PQQQ....C.RRR.C.");
		//String st = "A B B . . O A . P . . O X X P . . O . . P Q Q Q . . . . C . R R R . C . ";
		//st = st.replaceAll("\\s+","");
		//s.initilizeState(s);
		s.setParent(null);
		s.show();
		A_StarSolver solver = new A_StarSolver(s);
		System.out.println(solver.solve());
		
		int u = 1;
		while(!solver.ops.isEmpty()){
			
			Op oo = solver.ops.pop();
			System.out.println(oo.constructOperation());
			System.out.println(oo.constructOperation() + "\n\n"+ (u++) +"\n::::");
			solver.root.makeMove(oo);
			solver.root.draw();
			solver.root.show();
		}
		System.out.println("\n\n BF: "+nthroot(A_StarSolver.level, A_StarSolver.branching_factor));*/
	}
	public static int i = 1;
	public static void automation(String disc){
		/*if(i != 12 && i != 17 && i != 24 && i!= 33 && i!= 36 && i !=38 ){
			
			i++;
			return;
		}*/
		PrintStream out = null;
		try {
			out = new PrintStream(new FileOutputStream("results/"+i+".txt"));
			i++;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.setOut(out);
		System.out.println(disc);
		State s = new State();
		A_StarSolver solver = new A_StarSolver(s);
		s.initilizeState(disc);
		s.setParent(null);
		s.show();
		System.out.print(solver.solve());
		
		
		int si = solver.ops.size();
		while(!solver.ops.isEmpty()){
			
			Op oo = solver.ops.pop();
			System.out.println(oo.constructOperation());
			solver.root.makeMove(oo);
			solver.root.draw();
			solver.root.show();
		}
		System.out.println("\n\n BF: " +nthroot(A_StarSolver.level, A_StarSolver.branching_factor));
		System.out.println("\nNumber of moves: " + si + "  should be: " + optimal_solutions[i-2]);
		out.close();
	}
}

