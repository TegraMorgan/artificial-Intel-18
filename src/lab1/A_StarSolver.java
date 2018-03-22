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
import java.util.concurrent.TimeUnit;


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
	State root;
	static int branching_factor;
	static int level;
	Stack<Op> ops; 
	static String voo = "";
	static int[] optimal_solutions = new int[40];
	static Long max = Long.MIN_VALUE,min = Long.MAX_VALUE;
	static Long accumelated = (long) 0;
	
	public A_StarSolver(State initialState){
		this.root = initialState;
		OPEN_LIST = new FibonacciHeap<>();
		OPEN_LIST_HELPER = new HashMap<>();
		CLOSED_LIST = new HashMap<>();
		branching_factor = 0;
		level = 0;
		ops = new Stack<Op>();
	}

	/**
	 * A* algorithm implementation
	 * @return boolean
	 */
	
	public boolean solve(char h){
		
		root.setParent(null);
		root.g = 0;
		if(h =='1'){
			calculate_Fn(root);
		}
		else if(h == '2'){
			calculate_Fn2(root);
		}else{
			calculate_Fn3(root);
		}
		root.generatePossibleMoves();
		FibonacciHeapNode<State> rootHeapNode = new FibonacciHeapNode<State>(root, root.getFValue());
		OPEN_LIST.insert(rootHeapNode, root.getFValue());
		OPEN_LIST_HELPER.put(root.disc, rootHeapNode);
		
		while(!OPEN_LIST.isEmpty()){
			//System.out.println(OPEN_LIST.toString());
			level++;
			FibonacciHeapNode<State> min = OPEN_LIST.min();
			min = remove_min();
			CLOSED_LIST.put(min.data.disc, min);
			if(min.data.isGoal()){
				//System.out.println("Solution FOUND!");
				State path = min.data;
				while(path.getParent() != null){
					ops.push(path.getOpp());
					path = path.getParent();
				}
				branching_factor *= level;
				return true;
			}
			
			ArrayList<Op> operations = min.data.generatePossibleMoves().getPossibleOperations();
			for(Op op: operations){
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
				if(h =='1'){
					calculate_Fn(s);
				}
				else if(h == '2'){
					calculate_Fn2(s);
				}else{
					calculate_Fn3(s);
				}
				
				//Checks if the state was opened/closed previously
				
				boolean opened = OPEN_LIST_HELPER.containsKey(s.disc)
						,closed = CLOSED_LIST.containsKey(s.disc);
				FibonacciHeapNode<State> stateHelper = null;
				if(closed || opened){					
					stateHelper = (closed)? CLOSED_LIST.get(s.disc): OPEN_LIST_HELPER.get(s.disc);					
					if(s.getFValue() < stateHelper.key){
						if(opened){
							OPEN_LIST.delete(stateHelper);
							OPEN_LIST_HELPER.remove(stateHelper);
							stateHelper.data.setFValue(s.getFValue());
							insert_opened(stateHelper);
						}else{
							FibonacciHeapNode<State> fbn = CLOSED_LIST.remove(s.disc);
							fbn.data.setFValue(s.getFValue());
							insert_opened(fbn);
							
						}
					}
					continue;
				}
				branching_factor++;
				FibonacciHeapNode<State> new_fbn = new FibonacciHeapNode<State>(s, s.getFValue());
				insert_opened(new_fbn);
				
			}
		}
		return false;
	}
	
	public void insert_opened(FibonacciHeapNode<State> fbn) {
		OPEN_LIST.insert(fbn, fbn.data.getFValue());
		OPEN_LIST_HELPER.put(fbn.data.disc, fbn);
		
	}

	private void calculate_Fn(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
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
			//System.out.println("VALUE: " + (count + node.g) + " - " + i);
			node.setFValue(count + node.g);
		}
	}
	
	private void calculate_Fn2(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			double count = node.nextStates.size();
			//System.out.println("VALUE: " + (-count + node.g) + " - " + level);
			node.setFValue(-count + node.g);
		}
	}
	
	
	// default: .5:.5
	// case 4 : .8:.2
	// case 5 : .2:.8
	// case 6 : .01:.99 (plus level)
	private void calculate_Fn3(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			calculate_Fn(node);
			double count = node.nextStates.size();
			//System.out.println("VALUE: " + (-count + node.g) + " - " + level);
			node.setFValue(0.01*(-count + node.g) + 0.99*(node.getFValue() + level));
		}
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
		solveAll();
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
	
	public static void solveAll(){
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
		    @SuppressWarnings("resource")
			BufferedReader br2 = new BufferedReader(new FileReader("states.txt"));
		    PrintStream out = null;
			try {
				out = new PrintStream(new FileOutputStream("final_results/SOLS6.txt"));
				i++;
			} catch (FileNotFoundException e) {
				
				e.printStackTrace();
			}
			System.setOut(out);
		    long startTime = System.currentTimeMillis();
		    long MAX_RUN_TIME = 388000000;
		    int num = 0;
		    for(String line; (line = br2.readLine()) != null; ) {
		    	//System.out.println(line);
		        automation(line);
		        num++;
		        if(TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - startTime) > MAX_RUN_TIME){
		        	System.out.println("Statistics: ");
					System.out.println("Min opened nodes: " + min + " Max opened nodes: " + max + " Average: " + accumelated/num);
		        	System.out.println("Max Time reached, number of solved problems: " + num);
		        	out.close();
		        	return;
		        }
		    	
		    }
		    System.out.println("All problems were solved successfully!!");
		    System.out.println("Total Runtime: " + TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - startTime)/1000000.0 + " Sec.");
		    // line is not visible here.
		    br2.close();
		} catch (IOException e){
			e.printStackTrace();
		}
		
	}
	public static int i = 1;
	public static int countt = 0;
	public static void automation(String disc){
		
		
/*TODO		System.out.println(disc);
*/		State s = new State();
		A_StarSolver solver = new A_StarSolver(s);
		s.initilizeState(disc);
		s.setParent(null);
		
		solver.solve('3');
		int num_opened = OPEN_LIST_HELPER.size();
		if(num_opened < A_StarSolver.min){
			A_StarSolver.min = (long) num_opened;
		}else if(num_opened > A_StarSolver.max){
			A_StarSolver.max = (long) num_opened;
		}
		A_StarSolver.accumelated += num_opened;
		//System.setOut(out);
		int si = solver.ops.size();
		while(!solver.ops.isEmpty()){
			
			Op oo = solver.ops.pop();
			//TODO System.out.print(oo.constructOperation() + " ");
		}
/*TODO		System.out.println("");
*/		System.out.print(/*"\n\nBranching Factor: " +*/nthroot(A_StarSolver.level, A_StarSolver.branching_factor));
/*TODO		System.out.println("\nNumber of moves: " + si + "  vs. Provided solution: " + (i-2 <= 39?optimal_solutions[i-2]: 0));
*/		//System.out.println("Total Runtime: " + A_StarSolver.RUNTIME/(1000000.0) + " Sec");
		i++;
		System.out.println();
		if(i == 42){
			
			System.out.println("Statistics: ");
			System.out.println("Min opened nodes: " + min + " Max opened nodes: " + max + " Average: " + accumelated/40);
		}
		if(i == 43)
			s.show();
		//out.close();
	}
}

