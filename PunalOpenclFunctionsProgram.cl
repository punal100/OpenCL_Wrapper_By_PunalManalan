//Some basic kernel for testing purpose

__kernel void Add_Integers(__global int* ABC, __global int* B, __global int* C)
{
	int num = get_global_id(0);
	C[num] = ABC[num] + B[num];
}

__kernel void Sub_Integers(__global int* ABC, __global int* B, __global int* C)
{
	int num = get_global_id(0);
	((*(((C + num ))))) = *(ABC + num) - B[num];//This is valid
}

//This does more than multiply, which is probably not what one would expect
__kernel void Multiply_Integers(__global int* ABC, __global int* B, __global int* C, __local int* D, __private TotalLocal)
{
	int num = get_global_id(0);
	int loc = get_local_id(0);
	ulong Total = 0;

	int MaxLim = (num + 1) * TotalLocal;

	for(int i = (MaxLim - TotalLocal); i < MaxLim; ++i)
	{
		D[i] = 0;// Setting 0 to all// Probably not needed
	}

	for(int i = (MaxLim - TotalLocal); i < MaxLim; ++i)
	{
		D[i] = D[i] + ABC[num] * B[num];
	}	

	for(int i = (MaxLim - TotalLocal); i < MaxLim; ++i)
	{
		Total = Total + D[i];
		//Total = TotalLocal;
	}

	C[num] = Total;
}

//This does more than multiply, which is probably not what one would expect
__kernel void Multiply_IntegersTestTwo(__global int* ABC, __global int* B, __global int* C, __local int* D, __private TotalLocal)
{
	int num = get_global_id(0);
	int loc = get_local_id(0);
	ulong Total = 0;

	int MaxLim = (num + 1) * TotalLocal;

	for(int i = (MaxLim - TotalLocal); i < MaxLim; ++i)
	{
		D[i] = 0;// Setting 0 to all// Probably not needed
	}

	for(int i = (MaxLim - TotalLocal); i < MaxLim; ++i)
	{
		D[i] = D[i] + ABC[num] * B[num];
	}	

	for(int i = (MaxLim - TotalLocal); i < MaxLim; ++i)
	{
		Total = Total + D[i];
		//Total = TotalLocal;
	}

	C[num] = Total;
}