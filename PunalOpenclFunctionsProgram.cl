//Some basic kernel for testing purpose

__kernel void Add_Integers(__global int* A, __global int* B, __global int* C)
{
	int num = get_global_id(0);
	C[num] = A[num] + B[num];
}