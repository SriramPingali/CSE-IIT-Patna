import org.apache.spark._

import org.apache.spark.rdd.RDD
import org.apache.spark.util.IntParam
import org.apache.spark.sql.SQLContext
import org.apache.spark.graphx._
import org.apache.spark.graphx.util.GraphGenerators
import org.apache.spark.util.StatCounter

case class Flight(dofM: String, dofW: String, carrier: String, tailnum: String, flnum: Int, org_id: Long, origin: String, dest_id: Long, dest: String, crsdeptime: Double, deptime: Double, depdelaymins: Double, crsarrtime: Double, arrtime: Double, arrdelay: Double, crselapsedtime: Double, dist: Int)

object FlightApp {
def main(args: Array[String]):Unit={
//println("Hello,inside")
  def parseFlight(str: String): Flight = {
    val line = str.split(",")
    Flight(line(0), line(1), line(2), line(3), line(4).toInt, line(5).toLong, 
           line(6), line(7).toLong, line(8), line(9).toDouble, line(10).toDouble, 
           line(11).toDouble, line(12).toDouble, line(13).toDouble, line(14).toDouble, line(15).toDouble, line(16).toInt)
  }
  val conf = new SparkConf().setAppName("SparkDFebay")
  val sc = new SparkContext(conf)
  //Create RDD with the January 2014 data 
  val textRDD = sc.textFile("/home/iitp/flights/rita2014jan.csv")
  print("Inside TextRDD")
  textRDD.take(5).foreach(println)
  val flightsRDD = textRDD.map(parseFlight).cache()

  val airports = flightsRDD.map(flight => (flight.org_id, flight.origin)).distinct
  println("Airports")
  airports.take(5).foreach(println)

  // Defining a default vertex called nowhere
  val nowhere = "nowhere"

  val routes = flightsRDD.map(flight => ((flight.org_id, flight.dest_id), flight.dist)).distinct

  routes.cache

  // AirportID is numerical - Mapping airport ID to the 3-letter code
  val airportMap = airports.map { case ((org_id), name) => (org_id -> name) }.collect.toList.toMap

  //airportMap: scala.collection.immutable.Map[Long,String] = Map(13024 -> LMT, 10785 -> BTV, 14574 -> ROA, 14057 -> PDX, 13933 -> ORH,     	11898 -> GFK, 14709 -> SCC, 15380 -> TVC,

  // Defining the routes as edges
  val edges = routes.map { case ((org_id, dest_id), distance) => Edge(org_id.toLong, dest_id.toLong, distance) }

  //Defining the Graph
  val graph = Graph(airports, edges, nowhere)

  // LNumber of airports   
  val numairports = graph.numVertices
  println("vertices")
  graph.vertices.take(3).foreach(println)
  println("Edges")
  graph.edges.take(3)
  println("Distance between the airports is greater than 1000 miles")
  graph.edges.filter { case (Edge(org_id, dest_id, distance)) => distance > 1000 }.take(5).foreach(println)
  // res9: Array[org.apache.spark.graphx.Edge[Int]] = Array(Edge(10140,10397,1269), Edge(10140,10821,1670), Edge(10140,12264,1628))

  // Number of routes
  println("Number of routes")
  val numroutes = graph.numEdges
  print(numroutes)
  // The EdgeTriplet class extends the Edge class by adding the srcAttr and dstAttr members which contain the source and destination properties respectively.   
  graph.triplets.take(3).foreach(println)

  // Define a reduce operation to compute the highest degree vertex
  def max(a: (VertexId, Int), b: (VertexId, Int)): (VertexId, Int) = {
    if (a._2 > b._2) a else b
  }

  // we can compute the in-degree of each vertex (defined in GraphOps) by the following:
  // which airport has the most incoming flights?
  graph.inDegrees.collect.sortWith(_._2 > _._2).map(x => (airportMap(x._1), x._2))
  //res46: Array[(String, Int)] = Array((ATL,152), (ORD,145), (DFW,143), (DEN,132), (IAH,107), (MSP,96), (LAX,82), (EWR,82), (DTW,81), (SLC,80), 
  graph.outDegrees.join(airports).sortBy(_._2._1, ascending = false).take(1).foreach(println)
  val maxout = graph.outDegrees.join(airports).sortBy(_._2._1, ascending = false).take(10)
//res46: Array[(String, Int)] = Array((ATL,152), (ORD,145), (DFW,143), (DEN,132), (IAH,107), (MSP,96), (LAX,82), (EWR,82), (DTW,81), (SLC,80), 
  val maxIncoming = graph.inDegrees.collect.sortWith(_._2 > _._2).map(x => (airportMap(x._1), x._2)).take(10)
  println("Maximum Incoming Flights airports")
  maxIncoming.foreach(println)

  //maxout.foreach(println)
  println("Maximum outgoing flights airports")
  val maxOutgoing = graph.outDegrees.collect.sortWith(_._2 > _._2).map(x => (airportMap(x._1), x._2)).take(5)
  maxOutgoing.foreach(println)

  // What are the top 10 flights from airport to airport?
  println("Top 10 flights from airport to airport") 
  graph.triplets.sortBy(_.attr, ascending = false).map(triplet =>
    "There were " + triplet.attr.toString + " flights from " + triplet.srcAttr + " to " + triplet.dstAttr + ".").take(10).foreach(println)

  //Lowest 10 flights from airport to airport

  println("Lowest 10 flights from airport to airport") 
  graph.triplets.sortBy(_.attr).map(triplet =>
    "There were " + triplet.attr.toString + " flights from " + triplet.srcAttr + " to " + triplet.dstAttr + ".").take(10).foreach(println)
println("Using Pagerank")


  //Using pagerank for most important airports
  val rank = graph.pageRank(0.001).vertices
  //join the ranks with the map of airport id to name
  val temp = rank.join(airports)
  temp.take(5).foreach(println)

  //sorting the airports by ranking
  val temp2 = temp.sortBy(_._2._1,false)
  println("Sorted by ranking using pagerank")
  temp2.take(5).foreach(println)

  //getting just the airport names
  println("Important airport names by pagerank")
 val impAirPorts = temp2.map(_._2._2)
 impAirPorts.take(5).foreach(println)


}

}
 
