/*
 * OverlapGTest.cpp
 *
 *  Created on: 21.12.2012
 *      Author: cls
 */

#include "OverlapGTest.h"


namespace EnsembleClustering {

TEST_F(OverlapGTest, testRegionGrowingOverlapperOnOneClustering) {
	GraphGenerator graphGen;
	int64_t n = 10;
	Graph G = graphGen.makeCompleteGraph(n);

	ClusteringGenerator clusterGen;
	std::vector<Clustering> clusterings;
	int z = 3; // number of clusterings
	for (int i = 0; i < z; ++i) {
 		clusterings.push_back(clusterGen.makeOneClustering(G));
	}
	DEBUG("end of loop");

	RegionGrowingOverlapper over;
	Clustering core = over.run(G, clusterings);

	// test if core clustering is one-clustering
	node v = 1;
	cluster one = core.clusterOf(v);
	bool isOneClustering = true;
	G.forNodes([&](node v) {
		cluster c = core.clusterOf(v);
		DEBUG("CLUSTER! c = " << c);
		isOneClustering = isOneClustering && (c == one);
	});

	EXPECT_TRUE(isOneClustering) << "overlap of multiple 1-clustering should be a 1-clustering";

}


TEST_F(OverlapGTest, testRegionGrowingOverlapperOnSingletonClustering) {
	GraphGenerator graphGen;
	int64_t n = 10;
	Graph G = graphGen.makeCompleteGraph(n);

	ClusteringGenerator clusterGen;
	std::vector<Clustering> clusterings;
	int z = 3; // number of clusterings
	for (int i = 0; i < z; ++i) {
		Clustering zeta = clusterGen.makeSingletonClustering(G);
		clusterings.push_back(zeta);
	}

	RegionGrowingOverlapper over;
	Clustering core = over.run(G, clusterings);

	// test if core clustering is singleton-clustering
	bool isSingleton = true;
	G.forEdges([&](node u, node v) {
		isSingleton = isSingleton && (core.clusterOf(u) != core.clusterOf(v));
	});

	EXPECT_TRUE(isSingleton) << "overlap of multiple  singleton clusterings should be a singleton clustering";

}


TEST_F(OverlapGTest, testHashing) {
	std::hash<int64_t> h;
	for (int64_t i = 1; i <= 42; ++i) {
		size_t val = h(i);
		std::cout << val << " ";
	}
	std::cout << std::endl;

	size_t val = 42;
	size_t masked = val & 0xffff;
	std::cout << masked << std::endl;
}


TEST_F(OverlapGTest, testHashingOverlapperOnSingletonClusterings) {
	GraphGenerator graphGen;
	int64_t n = 10;
	Graph G = graphGen.makeCompleteGraph(n);

	ClusteringGenerator clusterGen;
	std::vector<Clustering> clusterings;
	int z = 3; // number of clusterings
	for (int i = 0; i < z; ++i) {
		clusterings.push_back(clusterGen.makeSingletonClustering(G));
	}

	HashingOverlapper over;
	Clustering core = over.run(G, clusterings);

	// test if core clustering is singleton-clustering
	bool isSingleton = true;
	G.forEdges([&](node u, node v) {
		isSingleton = isSingleton && (core.clusterOf(u) != core.clusterOf(v));
	});

	EXPECT_TRUE(isSingleton) << "overlap of multiple  singleton clusterings should be a singleton clustering";
}


TEST_F(OverlapGTest, testHashingOverlapperOnOneClusterings) {
	GraphGenerator graphGen;
	int64_t n = 10;
	Graph G = graphGen.makeCompleteGraph(n);

	ClusteringGenerator clusterGen;
	std::vector<Clustering> clusterings;
	int z = 3; // number of clusterings
	for (int i = 0; i < z; ++i) {
 		clusterings.push_back(clusterGen.makeOneClustering(G));
	}
	DEBUG("end of loop");

	HashingOverlapper over;
	Clustering core = over.run(G, clusterings);

	// test if core clustering is one-clustering
	node v = 1;
	cluster one = core.clusterOf(v);
	bool isOneClustering = true;
	G.forNodes([&](node v) {
		cluster c = core.clusterOf(v);
		DEBUG("CLUSTER! c = " << c);
		isOneClustering = isOneClustering && (c == one);
	});

	EXPECT_TRUE(isOneClustering) << "overlap of multiple 1-clustering should be a 1-clustering";

}


// FIXME: hash function produces collisions for small cluster IDs, vertex is moved to other cluster
// without being assigned before => assertion fails in moveToCluster
//TEST_F(OverlapGTest, testHashingOverlapperCollision) {
//	GraphGenerator graphGen;
//	int64_t n = 10;
//	Graph G = graphGen.makeCompleteGraph(n);
//
//	Clustering first(n);
//	Clustering second(n);
//
//	// assign nodes to 2 structurally identical but differently numbered clusters
//	G.forallNodes([&](node v){
//		if ((v % 2) == 0) {
//			first.moveToCluster(1, v);
//			second.moveToCluster(2, v);
//		} else {
//			first.moveToCluster(2, v);
//			second.moveToCluster(1, v);
//		}
//	});
//
//	std::vector<Clustering> clusterings;
//	clusterings.push_back(first);
//	clusterings.push_back(second);
//
//	HashingOverlapper over;
//	Clustering core = over.run(G, clusterings);
//
//	EXPECT_EQ(2, core.numberOfClusters()) << "there should be 2 clusters in the core clustering";
//}

} /* namespace EnsembleClustering */
