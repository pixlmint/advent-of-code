// filepath: AoC.Framework/Functional.cs
using System;
using System.Collections.Generic;
using System.Linq;

namespace AoC.Framework
{
    public static class Functional
    {
        // Map function
        public static IEnumerable<TResult> Map<TSource, TResult>(this IEnumerable<TSource> source, Func<TSource, TResult> selector)
        {
            return source.Select(selector);
        }

        // Filter function
        public static IEnumerable<T> Filter<T>(this IEnumerable<T> source, Func<T, bool> predicate)
        {
            return source.Where(predicate);
        }

        // Reduce function (left fold)
        public static TAccumulate Reduce<TSource, TAccumulate>(this IEnumerable<TSource> source, TAccumulate seed, Func<TAccumulate, TSource, TAccumulate> func)
        {
            TAccumulate result = seed;
            foreach (var item in source)
            {
                result = func(result, item);
            }
            return result;
        }

        // Immutable collection helpers
        public static IEnumerable<T> Append<T>(this IEnumerable<T> source, T value)
        {
            return source.Concat(new[] { value });
        }

        public static IEnumerable<T> Prepend<T>(this IEnumerable<T> source, T value)
        {
            return new[] { value }.Concat(source);
        }
    }
}