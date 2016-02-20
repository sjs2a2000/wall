def brickwall(w,h):
	# generate single brick layer of width w (by recursion)
	def gen_layers(w):
		if w in (0,1,2,3):
			return {0:[], 1:[], 2:[[2]], 3:[[3]]}[w]
		return [(layer + [2]) for layer in gen_layers(w-2)] + \
			   [(layer + [3]) for layer in gen_layers(w-3)]
	# precompute info about whether pairs of layers are compatible
	def gen_conflict_mat(layers, nlayers, w):
		# precompute internal brick positions for easy comparison
		def get_internal_positions(layer, w):
			acc = 0; intpos = set()
			for brick in layer:
				acc += brick; intpos.add(acc)
			intpos.remove(w)
			return intpos
		intpos = [get_internal_positions(layer, w) for layer in layers]		   
		mat = []
		for i in range(nlayers):
			mat.append([j for j in range(nlayers) \
							  if intpos[i].isdisjoint(intpos[j])])
		return mat
	layers = gen_layers(w)
	print 'layers:', layers
	nlayers = len(layers)
	mat = gen_conflict_mat(layers, nlayers, w)
	print 'mat:', mat

	# dynamic programming to recursively compute wall counts
	nwalls = nlayers*[1]
	nwalls2 = nlayers*[1]
	print 'h:',h
	for i in range(1,h):
		tempwalls=nlayers*[1]
		for j in range(nlayers):
			l=0
			for k in mat[j]:
				l+=nwalls[k] 
			tempwalls[j]=l
		nwalls=tempwalls
		print '1',nwalls
		nwalls2 = [sum(nwalls2[k] for k in mat[j]) for j in range(nlayers)]
		print '2', nwalls2
	return sum(nwalls)

if __name__=='__main__':
	print(brickwall(9,3))
