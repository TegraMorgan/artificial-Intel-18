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
import java.util.Scanner;
import java.util.Stack;
import java.util.Map.Entry;
import java.util.concurrent.TimeUnit;




public class A_StarSolver {	
	
	static FibonacciHeap<State> OPEN_LIST;
	static Map<String,FibonacciHeapNode<State> > OPEN_LIST_HELPER;
	static Map<String,FibonacciHeapNode<State>  > CLOSED_LIST;
	State root;
	static int branching_factor;
	static int level;
	int max_level;
	Stack<Op> ops; 
	static String voo = "";
	static int[] optimal_solutions = new int[40];
	static Long max = Long.MIN_VALUE,min = Long.MAX_VALUE;
	static Long accumelated = (long) 0;
	public static int i = 1;
	public static int countt = 0;
	public long total_time;
	public double heuristic_avg = 0;
	public int ignored_nodes = 0;
	
	//constructor, initializes the solver with appropriate parameters
	public A_StarSolver(State initialState){
		max_level = 0;
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

	public boolean solve(char h , long bound){
		final long start = System.currentTimeMillis();
		root.setParent(null);
		root.g = 0;
		if(h =='1'){
			calculate_Fn(root);
		}else if(h == '2'){
			calculate_Fn2(root);
		}else if(h == '3'){
			calculate_Fn4(root);
		}else{
			calculate_Fn0(root);
		}
		root.generatePossibleMoves();
		FibonacciHeapNode<State> rootHeapNode = new FibonacciHeapNode<State>(root, root.getFValue());
		OPEN_LIST.insert(rootHeapNode, root.getFValue());
		OPEN_LIST_HELPER.put(root.disc, rootHeapNode);
		
		while(!OPEN_LIST.isEmpty()){
			if(TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - start) > bound){
				total_time = bound;
				return false;
			}
			FibonacciHeapNode<State> min = OPEN_LIST.min();
			min = remove_min();
			CLOSED_LIST.put(min.data.disc, min);
			if(min.data.isGoal()){
				total_time = TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - start);
				level = min.data.g;
				State path = min.data;
				while(path.getParent() != null){
					ops.push(path.getOpp());
					path = path.getParent();
				}
				
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
					if(s.g > max_level){
						max_level = s.g;
					}
				}
				if(h =='1'){
					calculate_Fn(s);
				}else if(h == '2'){
					calculate_Fn2(s);
				}else if(h == '3'){
					calculate_Fn4(s);
				}else{
					calculate_Fn0(s);
				}
				
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
						stateHelper.data.g = s.g+1;
						if(s.g+1 > max_level){
							max_level = s.g + 1;
						}
					}
					ignored_nodes++;
					continue;
				}
				branching_factor++;
				FibonacciHeapNode<State> new_fbn = new FibonacciHeapNode<State>(s, s.getFValue());
				insert_opened(new_fbn);
				
			}
		}
		return false;
	}
	
	/**
	 * insert fbn into the open list
	 * @param fbn
	 */
	public void insert_opened(FibonacciHeapNode<State> fbn) {
		OPEN_LIST.insert(fbn, fbn.data.getFValue());
		OPEN_LIST_HELPER.put(fbn.data.disc, fbn);
		
	}

	/**
	 * (Null Function) heuristic function that gives every node the 
	 * g value  
	 * @param node
	 */
	private void calculate_Fn0(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			node.setFValue( node.g);
			
		}
	}
	
	/**
	 * (Distance Function) heuristic function that gives every node the 
	 * number of cars that blocks car XX from exit 
	 * @param node
	 */
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
			heuristic_avg += count;
			node.setFValue(count + node.g);
		}
	}
	
	/**
	 * (Contra-Mobility Function) heuristic function that gives every node 
	 * the number of the possible moves on the board   
	 * @param node
	 */
	private void calculate_Fn2(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			node.generatePossibleMoves();
			double count = node.nextStates.size();
			heuristic_avg += -count;
			node.setFValue(-count + node.g);
		}
	}
	
	/*private void calculate_Fn3(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			calculate_Fn(node);
			node.generatePossibleMoves();
			double count = node.nextStates.size();
			//System.out.println("VALUE: " + (-count + node.g) + " - " + level);
			node.setFValue(0.5*(-count + node.g) + 0.5*(node.getFValue()));
		}
	}*/
	
	
	/**
	 * (Contra-Mobility + Distance Function) heuristic function that gives every node the 
	 * a mixture value of 0.8*Contra-Mobility value + 0.2*Distance value 
	 * @param node
	 */
	private void calculate_Fn4(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			calculate_Fn(node);
			node.generatePossibleMoves();
			double count = node.nextStates.size();
			//System.out.println("VALUE: " + (-count + node.g) + " - " + level);
			heuristic_avg += (0.8*(-count) + 0.2*(node.getFValue() - node.g));
			node.setFValue(0.8*(-count + node.g) + 0.2*(node.getFValue()));
		}
	}
	
	/*private void calculate_Fn5(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			calculate_Fn(node);
			node.generatePossibleMoves();
			double count = node.nextStates.size();
			//System.out.println("VALUE: " + (-count + node.g) + " - " + level);
			node.setFValue(0.2*(-count + node.g) + 0.8*(node.getFValue()));
		}
	}*/
	
	
	/*private void calculate_Fn6(State node) {
		if(node.isGoal()){
			node.setFValue(-100.0);
		}else{
			calculate_Fn(node);
			node.generatePossibleMoves();
			double count = node.nextStates.size();
			//System.out.println("VALUE: " + (-count + node.g) + " - " + level);
			node.setFValue(0.01*(-count + node.g) + 0.99*(node.getFValue()));
		}
	}*/
	
	
	/**
	 * Delete minimum in fibonacci heap
	 * @return
	 */
	public FibonacciHeapNode<State> remove_min(){
		FibonacciHeapNode<State> min = OPEN_LIST.removeMin();	
		OPEN_LIST_HELPER.remove(min.data.disc);
		return min;
		
		
	}
	
	
	/**
	 * calculate the nth root of number A
	 * @param n
	 * @param A
	 * @return
	 */
	public static double nthroot(int n, double A) {
		return nthroot(n, A, .001);
	}
	
	
	/**
	 * calculate the nth root of number A
	 * @param n
	 * @param A
	 * @return
	 */
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
		System.out.println("## Red Car agent ##");
		System.out.println("Make sure that you provided the input problem inside the 'states.txt' file.\n");
		System.out.print("Enter time limit in seconds (float/integer): ");
		Scanner sc = new Scanner(System.in);
		double i = sc.nextDouble();		
		System.out.print("Choose a heuristic function(1-4):\n1. Distance\n2. Contra-Mobility\n3. Distance + Contra-Mobility (Our chosen solution)\n4. Null Function(Best Solution)\n..> ");
		char h = sc.next().charAt(0);
		System.out.println("The solution of the provided problem will appear in a new file named 'Sols.txt' in this directory.");
		solveAll((int)(i*1000000),h);
		sc.close();
		
	}
	
	/**
	 * solve all the tests and print it to .txt file into the project in final_result document 
	 * @param bound
	 * @param h
	 */
	public static void solveAll(long bound,char h){
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
		    PrintStream out = null;
			try {
				out = new PrintStream(new FileOutputStream("SOLS.txt"));
				i++;
			} catch (FileNotFoundException e) {
				
			}
			System.setOut(out);
		    long startTime = System.currentTimeMillis();
		    for(String line; (line = br2.readLine()) != null; ) {
		    	//System.out.println(line);
		        automation(line,bound,h);
		    }
		    
		    System.out.println("Total Runtime: " + TimeUnit.MILLISECONDS.toMicros(System.currentTimeMillis() - startTime)/1000000.0 + " Sec.");
		    // line is not visible here.
		    br2.close();
		    out.close();
		} catch (IOException e){
			e.printStackTrace();
		}
		
	}
	
	/**
	 * automation function that runs all the tests
	 * @param disc
	 * @param bound
	 * @param h
	 */
	public static void automation(String disc, long bound,char h){
		
		System.out.println("Problem "+ (++countt)+":");
		State s = new State();
		A_StarSolver solver = new A_StarSolver(s);
		s.initilizeState(disc);
		s.setParent(null);
		
		if(!solver.solve(h,bound)){
			System.out.println("Failed.");
		}
			
		int num_opened = OPEN_LIST_HELPER.size();
		if(num_opened < A_StarSolver.min){
			A_StarSolver.min = (long) num_opened;
		}else if(num_opened > A_StarSolver.max){
			A_StarSolver.max = (long) num_opened;
		}
		A_StarSolver.accumelated += num_opened;
		int si = solver.ops.size();
		while(!solver.ops.isEmpty()){
			
			Op oo = solver.ops.pop();
			System.out.print(oo.constructOperation() + " ");
		}
		System.out.println("");
		System.out.printf("\n\nBranching Factor: %.4f",nthroot(A_StarSolver.level, A_StarSolver.branching_factor));
		System.out.println();
		System.out.printf("Penetrance Rate: %.4f" , A_StarSolver.level/(double)A_StarSolver.branching_factor);
		System.out.println();
		System.out.println("\nNumber of moves: " + si + "  vs. Provided solution: " + (i-2 <= 39?optimal_solutions[i-2]: 0));
		System.out.printf("Heuristic average value assigned: %.2f" , (double)solver.heuristic_avg/(branching_factor + solver.ignored_nodes));
		System.out.println();
		int max_depth = Integer.MIN_VALUE,min_depth = Integer.MAX_VALUE,accumelated_depth = 0;
		for (Entry<String, FibonacciHeapNode<State>> fpn : A_StarSolver.OPEN_LIST_HELPER.entrySet()) {
			int depth = fpn.getValue().data.g;
			if(depth < min_depth){
				min_depth = depth;
			}else if(depth > max_depth){
				max_depth = depth;
			}
			accumelated_depth += depth;
		}
		System.out.println("Max Depth before cutoff: " + max_depth);
		System.out.println("Min Depth before cutoff: " + min_depth);
		System.out.printf("Average Depth before cutoff: %.2f" , accumelated_depth/(double)A_StarSolver.OPEN_LIST_HELPER.size());
		System.out.println();
		System.out.println("finished in: " + solver.total_time/1000000.0 + " sec.");
		i++;
		System.out.println();
		if(i == 42){
			
			System.out.println("Statistics: ");
			System.out.println("Min opened nodes after solution was found: " + min + " || Max opened nodes: " + max + " || Average: " + accumelated/40);
		}
	}
}

