#include "blur.h"
#include <iostream>
Device device = get_device();
Context context ({device});

Device get_device() {
	std::vector<Platform> platforms;
	Platform::get(&platforms);
	
	Platform platform = platforms[0];

	std::vector<Device> devices;
	Device device = devices[0];

	return device;
}

Program makeProgram(std::string file_name) {
	FILE *f = fopen(file_name.c_str(), "rb");

	fseek(f, 0, SEEK_END);

	int size = ftell(f);

	fseek(f, 0, SEEK_SET);

	char *src = new char[size + 1];

	fread(src, size, 1, f);

	fclose(f);

	Program::Sources srcs;

	srcs.push_back({src, size});
	
	Program program (context, srcs);

	if(program.build({device}) != CL_SUCCESS) {
		std::cout << "Error building program" << std::endl;		
	}

	return program;
}

Program blur_program = makeProgram("program.ocl");
Kernel blur_kernel = Kernel(blur_program, "blur");

void blur(std::vector<int> & a, std::vector<int> & b, std::vector<int> & c) { 
	int s = a.size() * sizeof(int);

	CommandQueue queue (context, device);
	
	Buffer buffer_a(context, CL_MEM_READ_WRITE, s);
	Buffer buffer_b(context, CL_MEM_READ_WRITE, s);
	Buffer buffer_c(context, CL_MEM_READ_WRITE, s);

	queue.enqueueWriteBuffer(buffer_a, CL_TRUE, 0, s, a.data());
	queue.enqueueWriteBuffer(buffer_b, CL_TRUE, 0, s, b.data());

	blur_kernel.setArg(0, buffer_a);
	blur_kernel.setArg(1, buffer_b);
	blur_kernel.setArg(2, buffer_c);

	queue.enqueueNDRangeKernel(blur_kernel, NullRange, NDRange(s), NDRange(1));

	queue.enqueueReadBuffer(buffer_c, CL_TRUE, 0, s, c.data());
	queue.finish();
}

