public class Node
{
		public int jobId;
		public int jobTime;
		public Node next; 
		
			Node() //Default Class Constructor
			{
				jobId = -1; //Use -1 as default to avod duplicates in the event that a job has an id of zero.
				jobTime = 0;
				next = null;
			}
			
			Node(int id, int time) //Parameterized constructor to create a node given the id and time.
			{
				jobId = id;
				jobTime = time;
				next = null;
			}
			


}
