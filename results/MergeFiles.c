{
	TFileMerger *fm = new TFileMerger;

	fm->OutputFile("AFRODITE.root");
	fm->AddFile("AFRODITE_t0.root");
	fm->AddFile("AFRODITE_t1.root");
	fm->AddFile("AFRODITE_t2.root");
	fm->AddFile("AFRODITE_t3.root");
	fm->AddFile("AFRODITE_t4.root");
	fm->AddFile("AFRODITE_t5.root");
	fm->AddFile("AFRODITE_t6.root");
	fm->AddFile("AFRODITE_t7.root");
	fm->Merge();
}