//Some basic kernel for testing purpose

__kernel void Add_Integers(__global int* ABC, __global int* B, __global int* C)
{
	int num = get_global_id(0);
	C[num] = ABC[num] + B[num];
}