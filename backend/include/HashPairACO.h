struct HashPairACO
{
    template <class T1, class T2, class T3, class T4>
    std::size_t operator()(const std::pair<std::pair<T1, T2>, std::pair<T3, T4>> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first.first);
        auto h2 = std::hash<T2>{}(p.first.second);
        auto h3 = std::hash<T3>{}(p.second.first);
        auto h4 = std::hash<T4>{}(p.second.second);

        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};