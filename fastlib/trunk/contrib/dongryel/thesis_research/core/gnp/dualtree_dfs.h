/** @file dualtree_dfs.h
 *
 *  A template generator for performing a depth first search dual-tree
 *  algorithm.
 *
 *  @author Dongryeol Lee (dongryel@cc.gatech.edu)
 */

#ifndef CORE_GNP_DUALTREE_DFS_H
#define CORE_GNP_DUALTREE_DFS_H

#include <boost/tuple/tuple.hpp>
#include <map>
#include "core/math/range.h"
#include "core/gnp/dualtree_trace.h"

namespace core {
namespace gnp {

/** @brief The dualtree algorithm template generator.
 */
template<typename ProblemType>
class DualtreeDfs {

  public:

    /** @brief The table type.
     */
    typedef typename ProblemType::TableType TableType;

    /** @brief The tree type.
     */
    typedef typename TableType::TreeType TreeType;

    /** @brief Global constants type for the problem.
     */
    typedef typename ProblemType::GlobalType GlobalType;

    /** @brief The type of result computed by the engine.
     */
    typedef typename ProblemType::ResultType ResultType;

  public:

    /** @brief An iterator object for iterative dual-tree computation.
     */
    template<typename IteratorMetricType>
    class iterator {
      private:

        /** @brief The type of iterator argument type for pushing onto
        	 *         the computation stack.
        	 */
        class IteratorArgType {

          private:

            /** @brief The query node.
             */
            TreeType *qnode_;

            /** @brief The reference node.
             */
            TreeType *rnode_;

            /** @brief The squared distance range associated with the
             *         query/reference pair.
             */
            core::math::Range squared_distance_range_;

          public:

            /** @brief The default constructor.
             */
            IteratorArgType();

            /** @brief The copy constructor.
             */
            IteratorArgType(const IteratorArgType &arg_in);

            /** @brief The constructor for the iterator argument that
             *         takes the starting pairs of query and reference
             *         nodes.
             */
            IteratorArgType(
              const IteratorMetricType &metric_in,
              TableType *query_table_in, TreeType *qnode_in,
              TableType *reference_table_in,
              TreeType *rnode_in);

            /** @brief The iterator argument constructor that takes
             *         the query-reference pairs and the squared
             *         distance between.
             */
            IteratorArgType(
              const IteratorMetricType &metric_in,
              TableType *query_table_in, TreeType *qnode_in,
              TableType *reference_table_in,
              TreeType *rnode_in,
              const core::math::Range &squared_distance_range_in);

            /** @brief Returns the query node.
             */
            TreeType *qnode();

            /** @brief Returns the query node.
             */
            TreeType *qnode() const;

            /** @brief Returns the reference node.
             */
            TreeType *rnode();

            /** @brief Returns the reference node.
             */
            TreeType *rnode() const;

            /** @brief Returns the squared distance range.
             */
            const core::math::Range &squared_distance_range() const;
        };

      private:

        /** @brief The query table.
         */
        TableType *query_table_;

        /** @brief The reference table.
         */
        TableType *reference_table_;

        /** @brief The associated dualtree engine.
         */
        DualtreeDfs<ProblemType> *engine_;

        /** @brief The associated metric.
         */
        const IteratorMetricType &metric_;

        /** @brief The result associated with the iterator.
         */
        ResultType *query_results_;

        /** @brief The computation stack trace for the iterative
         *         computation.
         */
        core::gnp::DualtreeTrace<IteratorArgType> trace_;

      public:

        /** @brief The iterator constructor.
         */
        iterator(
          const IteratorMetricType &metric_in,
          DualtreeDfs<ProblemType> &engine_in,
          ResultType *query_results_in);

        /** @brief Step forward the iterator.
         */
        void operator++();

        /** @brief Dereference the computed results.
         */
        ResultType &operator*();

        /** @brief Dereference the computed results.
         */
        const ResultType &operator*() const;

        /** @brief Finalize the iterator computation results.
         */
        void Finalize();
    };

  private:

    /** @brief The number of deterministic prunes.
     */
    int num_deterministic_prunes_;

    /** @brief The number of probabilistic prunes.
     */
    int num_probabilistic_prunes_;

    /** @brief The pointer to the problem.
     */
    ProblemType *problem_;

    /** @brief The starting query node.
     */
    TreeType *query_start_node_;

    /** @brief The query table.
     */
    TableType *query_table_;

    /** @brief The rank of the query set.
     */
    int query_rank_;

    /** @brief Starting reference node.
     */
    TreeType *reference_start_node_;

    /** @brief The reference table.
     */
    TableType *reference_table_;

    /** @brief The rank of the reference set.
     */
    int reference_rank_;

  private:

    /** @brief Performs the base case for a given node pair.
     */
    template<typename MetricType>
    void DualtreeBase_(
      const MetricType &metric,
      TreeType *qnode,
      TreeType *rnode,
      bool qnode_and_rnode_are_equal,
      ResultType *result);

    /** @brief Determines whether a pair of query/reference pair can
     *         be pruned deterministically.
     */
    bool CanSummarize_(
      TreeType *qnode,
      TreeType *rnode,
      bool qnode_and_rnode_are_equal,
      typename ProblemType::DeltaType &delta,
      const core::math::Range &squared_distance_range,
      typename ProblemType::ResultType *query_results);

    /** @brief Summarize a given pair of query/reference using a
     *         deterministic approximation.
     */
    void Summarize_(
      TreeType *qnode, TreeType *rnode,
      const typename ProblemType::DeltaType &delta,
      typename ProblemType::ResultType *query_results);

    template<typename MetricType>
    bool CanProbabilisticSummarize_(
      const MetricType &metric,
      TreeType *qnode,
      TreeType *rnode,
      bool qnode_and_rnode_are_equal,
      double failure_probability,
      typename ProblemType::DeltaType &delta,
      const core::math::Range &squared_distance_range,
      typename ProblemType::ResultType *query_results);

    /** @brief Employ a probabilistic summarization with the given
     *         probability level.
     */
    template<typename MetricType>
    void ProbabilisticSummarize_(
      const MetricType &metric,
      GlobalType &global,
      TreeType *qnode,
      double failure_probability,
      const typename ProblemType::DeltaType &delta,
      typename ProblemType::ResultType *query_results);

    /** @brief The heuristic for choosing one node over the other.
     */
    template<typename MetricType>
    void Heuristic_(
      const MetricType &metric,
      TreeType *node,
      TreeType *first_candidate,
      TreeType *second_candidate,
      TreeType **first_partner,
      core::math::Range &first_squared_distance_range,
      TreeType **second_partner,
      core::math::Range &second_squared_distance_range);

    /** @brief The canonical recursive case for dualtree depth-first
     *         algorithm.
     */
    template<typename MetricType>
    bool DualtreeCanonical_(
      const MetricType &metric,
      TreeType *qnode,
      TreeType *rnode,
      double failure_probability,
      const core::math::Range &squared_distance_range,
      ResultType *query_results);

    /** @brief Postprocess unaccounted contributions.
     */
    template<typename MetricType>
    void PostProcess_(
      const MetricType &metric,
      TreeType *qnode, ResultType *query_results,
      bool do_query_results_postprocess);

  public:

    static void PreProcess(
      TableType *query_table_in,
      TreeType *qnode,
      typename ProblemType::ResultType *query_results,
      double initial_pruned_in);

    /** @brief Preprocesses the reference tree.
     */
    static void PreProcessReferenceTree(
      GlobalType &global_in, TreeType *rnode);

    /** @brief The constructor.
     */
    DualtreeDfs();

    /** @brief Sets the starting query node for the dual-tree
     *         computation.
     */
    void set_query_start_node(TreeType *query_start_node_in);

    /** @brief Sets the starting reference node for the dual-tree
     *         computation.
     */
    void set_reference_start_node(TreeType *reference_start_node_in);

    void set_query_reference_process_ranks(
      int query_process_id, int reference_process_id);

    /** @brief Returns the number of deterministic prunes so far.
     */
    int num_deterministic_prunes() const;

    /** @brief Returns the number of probabilistic prunes so far.
     */
    int num_probabilistic_prunes() const;

    /** @brief Returns the pointer to the problem spec.
     */
    ProblemType *problem();

    /** @brief Returns the query table.
     */
    TableType *query_table();

    /** @brief Returns the reference table.
     */
    TableType *reference_table();

    /** @brief Gets an iterator object of the dualtree computation.
     */
    template<typename MetricType>
    typename DualtreeDfs<ProblemType>::template
    iterator<MetricType> get_iterator(
      const MetricType &metric_in,
      ResultType *query_results_in);

    /** @brief Initializes the dual-tree engine with a problem spec.
     */
    void Init(ProblemType &problem_in);

    /** @brief Computes the result.
     */
    template<typename MetricType>
    void Compute(
      const MetricType &metric,
      typename ProblemType::ResultType *query_results,
      bool do_initializations = true);
};
}
}

#endif
